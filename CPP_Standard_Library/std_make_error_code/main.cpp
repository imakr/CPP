#include <iostream>
using namespace std;
#include <system_error>
#include <cassert>

namespace mylib
{
    namespace errc {

        enum my_error
        {
            failed = 0
        };

        inline const char* error_message(int c)
        {
            static const char* err_msg[] =
            {
                "Failed",
            };

            assert(c < sizeof(err_msg) / sizeof(err_msg[0]));
            return err_msg[c];
        }

        class my_error_category : public std::error_category
        {
            public:

            my_error_category()
            { }

            std::string message(int c) const
            {
                return error_message(c);
            }

            inline const char* name() const noexcept override
            { return "My Error Category"; }

            const static error_category& get()
            {
                const static my_error_category category_const;
                return category_const;
            }
        };

    inline std::error_code make_error_code(my_error e)
    {
            return std::error_code(static_cast<int>(e), my_error_category::get());
    }

    } // end namespace errc
} // end namespace mylib

namespace std {

template<>
struct is_error_code_enum<mylib::errc::my_error>
    : std::true_type
{ };

} // end namespace std

int main()
{
    std::error_code ec1 = mylib::errc::make_error_code(mylib::errc::failed); // works
    std::error_code ec2 = mylib::errc::failed; // works
    bool result = (ec2 == mylib::errc::failed); // works
    std::string errmsg = ec1.message();
    std::cout << errmsg.c_str() << std::endl;
}
