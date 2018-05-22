#include <vector>
#include <string>
#include <iostream>
#include "highfive/H5File.hpp"
#include "highfive/H5DataSet.hpp"
#include "boost/multi_array.hpp"
#include "boost/gil/image.hpp"
#include "boost/gil/typedefs.hpp"
#include "boost/gil/extension/io/png_io.hpp"

namespace HF = HighFive;
using namespace boost::gil;

typedef boost::multi_array<float, 3> array_t;
typedef array_t::index index_t;

int main(int argc, char *argv[])
{
  // Read input file
  const std::string filename = "/home/alex/storage/Data/Sasha/discharge.zoom/flds.tot.209";
  HF::File file(filename, HF::File::ReadOnly);

  // Read dataset for electron density
  auto dataset = file.getDataSet("dens");
  auto dims = dataset.getSpace().getDimensions();
  array_t data(boost::extents[dims[0]][dims[1]][dims[2]]);
  dataset.read(data);

  std::cout << "(";
  for (auto& d : dims) {
    std::cout << d << ", ";
  }
  std::cout << ")" << std::endl;

  // Prepare png output
  const size_t width = 4096;
  const size_t height = 4096;
  unsigned char r[width * height];
  unsigned char g[width * height];
  unsigned char b[width * height];
  unsigned char a[width * height];

  boost::gil::rgb8c_planar_view_t view = boost::gil::planar_rgba_view(width, height, r, g, b, a, width);
  boost::gil::png_write_view("test.png", view);

  return 0;
}
