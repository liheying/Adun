
## Overview

自用进程注入器，目标是在目标进程中创建并执行线程

## Usage

./inject PID 

You can choose technique by giving parameter -d to use direct or -p to use process, the default technique is thread.
## Requirements

* x86_64 GNU/Linux (tested on Kali Linux 4.12.0-kali2-amd64)


## Building
```
git clone https://github.com/Srakai/Adun.git
cd Adun
make
```

