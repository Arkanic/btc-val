# val
Get value of list of bitcoin wallets (ie electrum addresses) by their public keys

Intended purpose is just to check your wallets every so often without actually exposing/using any private keys on a computer. Uses blockchain.info api to query address values and current price of btc

![image](https://user-images.githubusercontent.com/50847107/213012744-02ac0f84-36e2-4c4b-9520-d9ecd179b5e7.png)

*TODO: add bar-pie-chart thingy to show what addresses constitute the most btc, add more tokens, and different fiat values.*

Please create an issue if you want something in particular.

## Usage

`./val wallet.txt`

wallet.txt is a file of addresses separated by commas. All address formats work. i.e.:

```
34xp4vRoCGJym3xR7yCVPFHoCNxv4Twseo
bc1qgdjqv0av3q56jvd82tkdjpy7gdp9ut8tlqmgrpmv24sq90ecnvqqjwvw97
3JJmF63ifcamPLiAmLgG96RA599yNtY3EQ
1LruNZjwamWJXThX2Y8C2d47QqhAkkc5os
```

```
--help                Show this help message
--ticker [currency]   Set what fiat value the currency should be displayed in
--recenttx [amount]   Number of recent transactions to show. Will be limited to the number actually available.
--nortx               Do not show recent transactions
--noval               Do not show wallet value
```

## Building
Dependencies are libcurl-dev (web requests) and libjson-c-dev (json parsing), can be installed via `sudo apt-get install libcurl-4-dev libjson-c-dev`

Build:

```
make
./val // finished code
```


### Made by Arkanic
