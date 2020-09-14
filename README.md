# Fast Reliable File Transfer
> Intenert and Cloud Computing Lab3 (Fall 2020)

 [![license](https://img.shields.io/github/license/nhn/tui.editor.svg)](https://github.com/nhn/tui.editor/blob/master/LICENSE)

### Team Member

| Name | URL |
| --- | --- |
| Yibo Wag| [`Github Link`](https://github.com/LoneRan) |
| Yiran Jiang| [`Github Link`](https://github.com/Yiran-Jiang) |

## 📜 License

This test software is licensed under the MIT © [LoneRan](https://github.com/LoneRan).

## Install
OS: Ubuntu
Make sure your machine have gcc and g++ and other essentials installed.
Then just type `make` in terminal, it will generate two excutable files

## Usage
On server side, to start the server:
`./server [-p port_number]`
e.g. `./server -p 8000` and the server will be waiting at port 8000.


On client side:
`./client [-d destination_ip_address] [-p port_number] [-f file_name]`
e.g. `./client -d 3.129.62.167 -p 8000 -f data.bin` and the cilent will connect to exact server as the IP address specifies.


Use the following command to change the delay and loss rate:
`sudo tc qdisc change dev ens5 root netem limit 100000 delay 100ms loss 1%`


## Clean up
Type `make clean` to remove all "*.o" and excutable files.
