/**
  *  @file       Typedefs.h (header file)
  *  @version    1.0.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       21.04.2019 (release)
  *  @brief      Typedefs header file.
  *              File represents the basic and complex data types that can be
  *              used in the current project code. The purpose of this file
  *              implementation and further usage is for more code
  *              understandable.
  **/

#ifndef __CORE_TYPEDEFS_H
#define __CORE_TYPEDEFS_H

#ifndef __cplusplus
    #include <stdbool.h>
#else
    #include <string>
    #include <vector>
#endif 

/* Basic data types re-definition. */
typedef      unsigned char       u8;
typedef      unsigned short      u16;
typedef       unsigned int       u32;
typedef    unsigned long long    u64;
typedef           char           sym;
typedef          short           s16;
typedef           int            s32;
typedef        long long         s64;
typedef          float           f32;
typedef         double           f64;

/* Non-regular and complex data types re-definition. */
typedef        bool       flag;
#ifdef __cplusplus
    typedef      std::string        str;

    /* Vector type re-definitions. */
    typedef     std::vector<u8>     vu8;
    typedef    std::vector<u16>     vu16;
    typedef    std::vector<u32>     vu32;
    typedef    std::vector<u64>     vu64;
    typedef    std::vector<s16>     vs16;
    typedef    std::vector<s32>     vs32;
    typedef    std::vector<s64>     vs64;
    typedef    std::vector<f32>     vf32;
    typedef    std::vector<f64>     vf64;
    typedef    std::vector<flag>    vflag;
    typedef    std::vector<str>     vstr;
#endif

/* Some other re-definitions. */
#define    yes    true
#define    no     false

#endif // __CORE_TYPEDEFS_H