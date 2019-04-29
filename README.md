# QT Optirun Indicator (qtoptirunindicator)

NVIDIA Optimus BumbleBee Status Indicator for linux

## Summary

Short prehistory. New laptop and new energy saving fetures.

I've tried to install NVIDIA proprietary drivers from rpmfusion and negativo17 (on Fedora28) and ended up all the time with the same result black screen just right after the X server tried to start. The problem was new laptop with mx150 nvidia card.

So after googling I've found that I need to setup some service to handle switching between intel and nvidia card. After following by this guide and installing cusom repo with bumblebee service https://docs.fedoraproject.org/en-US/quick-docs/bumblebee/ everything started to work fine.

Fine but excluding one thing I don't see the current state of cards. Again googling I've found that there is no indicator, there is some KDE widget but for me it's unacceptable since I've moved out from KDE due to their new cool fetures (just for info as for me kde4 was great but not the current one). So as I'm using Mate I need some small program to show me status of my cards without relation to desktop manager.

So after spending few hours I've got this result. Simple and small program :) 

## Usage

Simply lanch it and you will see icon in systemtray.

Click on icon to show details.

Icon will change depends on current video card state:

Bumplebee is not running

<img src="https://user-images.githubusercontent.com/25594311/56906254-82071e80-6aaa-11e9-936c-c5a35b434fda.png" width="60%"></img> 

Bumplebee running is running

<img src="https://user-images.githubusercontent.com/25594311/56906262-84697880-6aaa-11e9-8528-10b93e90212c.png" width="60%"></img> 

To close program you should perform right click and select quit from menu.

## Technical Info

So I've used this repo to setup my system and write this small tool

```shell
chenxiaolong/bumblebee
```

So actually currently this tool relies on only one command:

```shell
$ optirun --status
Bumblebee status: Ready (3.2.1). X inactive. Discrete video card is off.

$ optirun --status
Bumblebee status: Ready (3.2.1). X is PID 8806, 1 applications using bumblebeed.
```

If your output looks similar then this program should work for you too.

Program update interval 1 second, it's possible to change but currently no way to save and restore settings, cause simply no any configuration files :)

## Instalation

Suppose you have installed QT5 (on fedora you can do that # dnf install qt5*):
 1. Create dir to build project and change current dir to created one:
 
 ```shell
 $ cd ~
 
 $ mkdir proj
 
 $ cd proj
 ```
 
 2. Download sources to your project dir and browse to it:
 
 ```shell
 $ git clone https://github.com/coozoo/qtoptirunindicator
 
 $ cd qtoptirunindicator
 ```
 3. Compile by executing next commands (maybe you need to use qmake if default is qt5):
 
 ```shell
 $ qmake-qt5
 
 $ make
 ```

That's all now you can run:

```shell
$ ./qtoptirunindicator
```

## maybe it will be extended with some fetures
