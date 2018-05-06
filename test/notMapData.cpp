
#include <string>
#include <utility>
#include <vector>

namespace qmj {
namespace test {

template <typename Std_container, typename Qmj_constainer>
class Test_not_map {
 public:
  Test_not_map();
  ~Test_not_map();

 protected:
  size_t data_size;
  Std_container std_con;
  Qmj_container qmj_con;
}

}  // namespace qmj
}  // namespace test