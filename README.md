# rsakey-cracker
Credits to: https://github.com/quarantin/rsakey-cracker

## Overview:
Tool to brute-force passphrase of RSA private keys in PEM format.

## Example:

```bash
$ cat dict.txt | rsakey-cracker ./private-key.pem
```

OR

```bash
$ john -i --stdout | rsakey-cracker ./private-key.pem
```

## Signals:
```bash
$ ./rsakey-cracker ../key.rsa <~ wordlists/rockyou.txt &
[1] 43014
$ kill -INFO 43014
current passphrase: "porn" key: ../key.rsa
```

