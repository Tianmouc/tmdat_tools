# tmdat_tools
data utilities for tianmouc v1

## build

```bash
sh compie.sh
```

## usage

check the parameter by: 

```bash
./tmdat_cut
```

- exmaple 1 : sync mode

```bash
./tmdat_cut --input '/home/lyh/Downloads/origin' --output '/home/lyh/Downloads/output' --sync --c1 5 --c2 15
```
- exmaple 2 : cut aop only

```bash
./tmdat_cut --input '/home/lyh/Downloads/origin' --output '/home/lyh/Downloads/output' --aop --a1 33 --a2 1000
```
- exmaple 3 : cut cop only

```bash
./tmdat_cut --input '/home/lyh/Downloads/origin' --output '/home/lyh/Downloads/output' --cop --c1 33 --c2 1000
```

## python api

TODO

you can use the .so file in ./lib if you need one