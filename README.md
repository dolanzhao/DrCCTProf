# DrCCTProf

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/696979ad8dff44c792632845a28cd2cf)](https://app.codacy.com/manual/dolanzhao/DrCCTProf_SC20?utm_source=github.com&utm_medium=referral&utm_content=dolanzhao/DrCCTProf_SC20&utm_campaign=Badge_Grade_Dashboard)

## Instructions for building DrCCTProf

### Linux

To build DrCCTProf on Linux, use the following commands as a guide. This builds 64-bit DrCCTProf in release mode:

```
# Install dependencies for Ubuntu 15+.  Adjust this command as appropriate for
# other distributions (in particular, use "cmake3" for Ubuntu Trusty).
$ sudo apt-get install cmake g++ g++-multilib doxygen transfig imagemagick ghostscript git zlib1g-dev
# Get sources.
$ git clone --recurse https://github.com/dolanzhao/DrCCTProf.git
# Then, simply type "sh build.sh" This will configure, make, and check DrCCTProf.
```
