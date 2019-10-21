
#include "task_ref.hpp"
#include "types.hpp"

#include <scoped_timer.hpp>

#include <boost/mp11.hpp>
using namespace boost::mp11;

#include <random>
#include <memory>
#include <algorithm>
#include <variant>
#include <boost/mp11/detail/mp_append.hpp>
#include <boost/mp11/list.hpp>

constexpr auto numTypes = 9;
constexpr auto testSize = 100000;



void checkAndSet(A& arr, std::string_view where)
{
    auto s = std::accumulate(arr.begin(), arr.end(), 0);
    if(s != testSize)
        fmt::print("Error in {}\n", where);
    std::fill(arr.begin(), arr.end(), 0);
}


using L = mp_list<X1, X2, X3, X4, X5, X6, X7, X8, X9>;
using my_variant = mp_rename<mp_push_front<L, mono>, std::variant>;


template<class F, class V>
auto simple_visit(F f, V&& v)
{
    constexpr auto size = std::variant_size_v<std::remove_reference_t<V>>;
    return mp_with_index<size>(v.index(), [&](auto I){return f(std::get<I>((V&&)v));});
}


int main() {


    std::array<int, 9> arr;
    std::fill(arr.begin(), arr.end(), 0);

    std::random_device device{};
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> distr(0, numTypes - 1);

    for(int i = 0; i < 1000; ++i) {
        {
            std::vector<std::unique_ptr<B>> v(testSize);
            std::generate(v.begin(), v.end(), [&] {
                return mp_with_index<numTypes>(distr(engine),
                                               [&](auto I) {
                                                   auto p = std::make_unique<mp_at_c<L, I()>, A&>(arr);
                                                   return std::unique_ptr<B>(std::move(p));
                                               });
            });

            {
                ScopedTimer timer("OOP");
                for (const auto& ptr: v) {
                    ptr->execute();
                }
            }

            checkAndSet(arr, "OOP");
        }

        {
            std::vector<my_variant> v(testSize);
            std::generate(v.begin(), v.end(), [&] {
                return mp_with_index<numTypes>(distr(engine),
                                               [&](auto I) {
                                                   return my_variant(mp_at_c<L, I()>(arr));
                                               });
            });

            {
                ScopedTimer timer("Standard Variant");
                for (const auto &var: v) {
                    std::visit([](auto &&x) { x.execute(); }, var);
                }
            }

            checkAndSet(arr, "Standard Variant");
        }

        {
            std::vector<my_variant> v(testSize);
            std::generate(v.begin(), v.end(), [&] {
                return mp_with_index<numTypes>(distr(engine),
                                               [&](auto I) {
                                                   return my_variant(mp_at_c<L, I()>(arr));
                                               });
            });

            {
                ScopedTimer timer("Simple Variant");
                for (const auto& var: v) {
                    simple_visit([](auto &&x) { x.execute(); }, var);
                }
            }

            checkAndSet(arr, "Simple Variant");
        }

        {
            std::vector<std::unique_ptr<B>> v1(testSize);
            std::vector<TaskRef> v2(testSize);

            for (int i = 0; i < testSize; ++i) {
                auto up = mp_with_index<numTypes>(distr(engine), [&](auto I) {
                    auto p = std::make_unique<mp_at_c<L, I()>, A&>(arr);
                    v2[i] = TaskRef(*p);
                    return std::unique_ptr<B>(std::move(p));
                });

                v1[i] = std::move(up);
            }

            {
                ScopedTimer timer("TaskRef");
                for (const auto& task: v2) {
                    task.execute();
                }
            }

            checkAndSet(arr, "TaskRef");
        }

        {
            std::vector<std::unique_ptr<B>> v1(testSize);
            std::vector<SmallTaskRef<L>> v2(testSize);

            for (int i = 0; i < testSize; ++i) {
                auto up = mp_with_index<numTypes>(distr(engine), [&](auto I) {
                    auto p = std::make_unique<mp_at_c<L, I()>, A&>(arr);
                    v2[i] = SmallTaskRef<L>(*p);
                    return std::unique_ptr<B>(std::move(p));
                });
                v1[i] = std::move(up);
            }

            {
                ScopedTimer timer("SmallTaskRef");
                for (const auto& task: v2) {
                    task.execute();
                }
            }

            checkAndSet(arr, "SmallTaskRef");
        }

    }

    ScopedTimer::printStatistics<std::micro>();
}
