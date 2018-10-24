/* work around some libpng errors */
#define int_p_NULL (int *)NULL
#define BOOST_DISABLE_ASSERTS

#include "boost/gil/extension/io/png.hpp"
#include "boost/gil/image.hpp"
#include "boost/gil/typedefs.hpp"
#include "boost/multi_array.hpp"
#include "highfive/H5DataSet.hpp"
#include "highfive/H5File.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#define png_infopp_NULL (png_infopp) NULL

namespace HF = HighFive;
using namespace boost::gil;

typedef boost::multi_array<float, 3> array_t;
typedef array_t::index index_t;

inline int clamp(float val, float max) {
  return std::min(static_cast<int>(std::abs(val) * 256.0 / max), 255);
}

const std::size_t res = 512;
const std::size_t tile_x = 32;
const std::size_t tile_y = res / tile_x;
const std::size_t width = tile_x * res;
const std::size_t height = tile_y * res;

int main(int argc, char *argv[]) {
  // Read input file
  const std::string filename =
      "/home/alex/storage/Data/Sasha/discharge.zoom/flds.tot.209";
  HF::File file(filename, HF::File::ReadOnly);

  // Read dataset for electron density
  auto dataset_e = file.getDataSet("dens");
  auto dataset_i = file.getDataSet("densi");
  auto dims = dataset_e.getSpace().getDimensions();
  array_t data_e(boost::extents[dims[0]][dims[1]][dims[2]]);
  array_t data_i(boost::extents[dims[0]][dims[1]][dims[2]]);
  dataset_e.read(data_e);
  dataset_i.read(data_i);
  std::cout << "Finished reading hdf data" << std::endl;

  // std::cout << "(";
  // for (auto& d : dims) {
  //   std::cout << d << ", ";
  // }
  // std::cout << ")" << std::endl;

  // Prepare png output
  std::vector<unsigned char> r(width * height, 0);
  std::vector<unsigned char> g(width * height, 0);
  std::vector<unsigned char> b(width * height, 0);
  std::vector<unsigned char> a(width * height, 0);

  std::cout << "Finished initializing data" << std::endl;

  int offset = (dims[0] - res) / 2;
  std::cout << offset << std::endl;
  // assign pixel values
  for (std::size_t k = 0; k < res; k++) {
    std::size_t k1 = k % tile_x;
    std::size_t k2 = k / tile_x;
    for (std::size_t j = 0; j < res; j++) {
      for (std::size_t i = 0; i < res; i += 8) {
        float val_i = data_i[k + offset][j + offset][i + offset];
        float val_e = data_e[k + offset][j + offset][i + offset];
        // if (val_i > val_e)
        //   std::cout << val_i << ", " << val_e << std::endl;
        // int v = std::max(std::min(static_cast<int>((val +
        // 100.0)*256.0/200.0), 255), 0);
        r[(k2 * res + j) * width + (k1 * res + i)] = clamp(val_i, 100.0);
        g[(k2 * res + j) * width + (k1 * res + i)] =
            clamp(val_i - val_e + 50.0, 100.0);
        b[(k2 * res + j) * width + (k1 * res + i)] = clamp(val_e, 100.0);
        a[(k2 * res + j) * width + (k1 * res + i)] =
            clamp(val_i + val_e, 100.0);
      }
    }
  }

  boost::gil::rgba8c_planar_view_t view = boost::gil::planar_rgba_view(
      width, height, r.data(), g.data(), b.data(), a.data(), width);
  boost::gil::write_view(std::string("test") + std::to_string(res) + ".png",
                         view, png_tag());

  return 0;
}
