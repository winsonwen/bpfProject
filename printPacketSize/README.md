# using for print out the size of each packet via BPF

1. Enviroment Setup
  Ubuntu: 18.04.1 LTS
  Linux: 4.15.0-109-generic

  - Dowload clang、LLVM、iproute2、libelf-dev
  ```bash
  # for ubuntu
  apt install clang llvm libelf-dev iproute2
  # test clang
  clang -v
  # test llvm
  llc --version
  # test iproute2
  ip link
  ```

  Download linux-source code and compile, setup bpf
  ```bash
  # make sure kernel version
  uname -r
  # Download the related source code
  apt-cache search linux-source
  apt install linux-source-5.3.0
  apt install libelf-dev

  cd /usr/src/linux-source-5.3.0
  tar xjf linux-source-5.3.0.tar.bz2
  cd linux-source-5.3.0/tools
  make -C  bpf/bpftool/
  cd bpf/bpftool/
  ./bpftool prog/net
  ```
  
  2. Related operations
  ```bash
    # Using clang comand to compile
    clang -O2 -target bpf -c xdp_bpf.c -o xdp_bpf.o

    # Loading the progame to the XDP Hook
      ip link set dev ens33 xdp obj xdp_bpf.o sec xdp

    # Detach from XDP hook
       ip link set dev ens33 xdp off
  
  ```
  
  3. result 
    We can verify output via a debug file:
      cat /sys/kernel/debug/tracing/trace_pipe
    or
      tail -f /sys/kernel/debug/tracing/trace
      
  ![bpf](https://github.com/winsonwen/bpfProject/blob/master/img/result.png)
  
  
  
  
  
  
  
  
  
  
  
  
  
  
