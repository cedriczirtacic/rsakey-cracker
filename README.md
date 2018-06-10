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
$ ./rsakey-cracker out.pem < ~/rockyou.txt &
[1] 83397
$ kill -INFO %1
key: out.pem, tasks: 10
time since started: 1 minutes, 3 seconds
```

