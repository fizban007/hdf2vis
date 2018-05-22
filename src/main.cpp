#include <vector>
#include <string>
#include <iostream>
#include "highfive/H5File.hpp"
#include "highfive/H5DataSet.hpp"
#include "boost/multi_array.hpp"

namespace HF = HighFive;

typedef boost::multi_array<double, 3> array_t;
typedef array_t::index index_t;

int main(int argc, char *argv[])
{
  const std::string filename = "/home/alex/storage/Data/Sasha/discharge.zoom/flds.tot.209";
  HF::File file(filename, HF::File::ReadOnly);

  auto dataset = file.getDataSet("dens");
  std::cout << dataset.getSpace().getDimensions() << std::endl;
  return 0;
}
