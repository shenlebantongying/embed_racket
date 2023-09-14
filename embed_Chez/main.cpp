#include <cstring>
#include <iostream>
#include <cstdlib>
#include "scheme.h"

void fail(){
    std::cout<<"Scheme related things breaks"<<std::endl;
}

static char *path_replace_filename(const char *s, const char *new_file)
{
  int len1 = strlen(s), len2 = strlen(new_file);
  char *r;

  while ((len1 > 0) && (s[len1-1] != '/') && (s[len1-1] != '\\'))
    len1--;

  r = static_cast<char *>(malloc(len1 + len2 + 1));
  memcpy(r, (void *)s, len1);
  memcpy(r+len1, (void *)new_file, len2+1);

  return r;
}

int main(int argc,const char* argv[])
{

    Sscheme_init(&fail);

    Sregister_boot_file(path_replace_filename(argv[0],"petite.boot"));
    Sregister_boot_file(path_replace_filename(argv[0],"scheme.boot"));

    Sbuild_heap(0, 0);

    // Sscheme_start(argc, argv); this starts interactive

    ptr one = Sinteger32(1);
    ptr two = Sinteger32(2);
    ptr plus = Stop_level_value(Sstring_to_symbol("+"));
    ptr ret = Scall2(plus,one,two);

    if(Sfixnump(ret)){
        std::cout<< "ret val is -> " << Sfixnum_value(ret) <<std::endl;
    }

    Sscheme_deinit();

    return 0;
}
