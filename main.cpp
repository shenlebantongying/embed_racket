#include <cstring>
#include <iostream>

#include <chezscheme.h>
#include <racketcs.h>
#include "base.c"

int main(int argc, char* argv[])
{

    racket_boot_arguments_t ba;

    memset(&ba, 0, sizeof(ba));

    char* exe_path = racket_get_self_exe_path(argv[0]);
    ba.boot1_path = racket_path_replace_filename(exe_path, "petite.boot");
    ba.boot2_path = racket_path_replace_filename(exe_path, "scheme.boot");
    ba.boot3_path = racket_path_replace_filename(exe_path, "racket.boot");

    ba.exec_file = argv[0];

    racket_boot(&ba);

    declare_modules();

    racket_namespace_require(Sstring_to_symbol("racket/base"));

    // Load Custom code in memory
    std::string myProg = R"(
(define (myPrint str)
  (begin (writeln str)
         (string-append "This got passed -> " str)))
)";

    racket_embedded_load_bytes(myProg.c_str(), myProg.length(), false);

    // Eval something
    ptr e = Scons(Sstring_to_symbol("myPrint"), Scons(Sstring("Print this sentence."), Snil));
    ptr ret_val = racket_eval(e);

    // Process return value
    if (Spairp(ret_val)) {
        ptr ret_car = Scar(ret_val);
        ptr ret_cdr = Scdr(ret_val);

        std::cout << "ret car is String? " << Sstringp(ret_car) << std::endl;
        std::cout << "ret cdr is Null? " << Snullp(ret_cdr) << std::endl;

        // Loop thorough returned string, and obtain the returned string.
        // Notice that chez internally uses UTF-32
        std::u32string ret_str;
        ret_str.reserve(Sstring_length(ret_car));
        for (int i = 0; i < Sstring_length(ret_car); ++i) {
            auto what = Sstring_ref(ret_car, i);
            ret_str += (char32_t)what;
        }

        std::string back_to_char(ret_str.begin(), ret_str.end());
        std::cout << "ret str is " << back_to_char << std::endl;
    }

    // Process data via bytevector (which is similar to std::string than Sstring)
    std::string ori_data("This is a long sentence");

    ptr byteVec = Smake_bytevector(ori_data.length(), 0);
    Slock_object(byteVec);
    memcpy(Sbytevector_data(byteVec), ori_data.data(), ori_data.length());
    Sunlock_object(byteVec);

    std::string recovered_data;
    recovered_data.reserve(Sbytevector_length(byteVec));
    recovered_data.assign((char*)Sbytevector_data(byteVec), Sbytevector_length(byteVec));
    std::cout << "rec data " << recovered_data << std::endl;

    return 0;
}
