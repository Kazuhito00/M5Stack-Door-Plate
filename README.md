# M5Stack-Door-Plate
M5Stackを使用したドアプレートアプリです。<br>
同一LAN内のブラウザから「仕事中」「会議中」「休憩中」の表示切替が出来ます。<br><br>
![i3am8-3991r](https://user-images.githubusercontent.com/37477845/168436914-5100296b-91ae-4fed-a204-64cb43a637ab.gif)

# Usage
1. [config.ini](sd/config.ini)の「YOUR_SSID」と「YOUR_SSID_PASSWORD」を、ご自身のWi-Fiの設定に変更してください
1. [sd](sd)ディレクトリ内のファイルをSDカードに書き込み、M5Stackに差し込んでください
1. [m5stack_door_plate.ino](m5stack_door_plate/m5stack_door_plate.ino)をArduino IDEで開き、M5Stackへプログラムを描きこんでください
1. プログラム起動後にM5Stackのディスプレイに表示される「http\://xxx.xxx.xxx.xxx/」をブラウザから開いてください

# Author
高橋かずひと(https://twitter.com/KzhtTkhs)
 
# License 
M5Stack-Door-Plate is under [Apache 2.0 License](LICENSE).<br><br>
