ifconfig br-lan down
brctl delbr br-lan
ifconfig eth0 10.107.48.52
wlanconfig ath0 destroy
wlanconfig ath0 create wlandev wifi1 wlanmode monitor
sleep 5
ath=`ifconfig -a| grep "ath" | cut -f 1 -d ' '`
iwconfig $ath channel 7
iwconfig $ath essid meas
iwconfig $ath txpower 23dbm
iwconfig $ath rate 6M fixed
ifconfig $ath down
ifconfig $ath 192.168.0.2 netmask 255.255.0.0 up
cat /root/config.cfg > /proc/net/madwifi/ath0/fractel_config
echo 0 > /proc/sys/dev/wifi1/diversity 
echo 1 > /proc/sys/dev/wifi1/rxantenna 
echo 1 > /proc/sys/dev/wifi1/txantenna
sysctl -w dev.wifi1.disable_cca=7
sysctl -w dev.wifi1.acktimeout=30
sysctl -w dev.wifi1.intmit=0
sysctl -w dev.wifi1.ofdm_weak_det=1
sysctl -w dev.wifi1.noise_immunity=4
rm /root/outfile
fractel_readlog /root/outfile &
fractel_routing_daemon /root/routing_log &
