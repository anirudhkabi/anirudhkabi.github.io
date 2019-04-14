echo hi > /root/test
wlanconfig ath0 destroy
wlanconfig ath0 create wlandev wifi0 wlanmode monitor
sleep 5
ath=`ifconfig -a| grep "ath" | cut -f 1 -d ' '`
iwconfig $ath channel 7
iwconfig $ath essid meas
iwconfig $ath txpower 27dbm
iwconfig $ath rate 6M fixed
ifconfig $ath down
cat /root/config.cfg > /proc/net/madwifi/ath0/fractel_config
ifconfig $ath 192.168.1.2 netmask 255.255.240.0 up
echo 0 > /proc/sys/dev/wifi0/diversity 
echo 1 > /proc/sys/dev/wifi0/rxantenna 
echo 1 > /proc/sys/dev/wifi0/txantenna
sysctl -w dev.wifi0.disable_cca=7
sysctl -w dev.wifi0.acktimeout=30
sysctl -w dev.wifi0.intmit=0
sysctl -w dev.wifi0.ofdm_weak_det=1
sysctl -w dev.wifi0.noise_immunity=4
#rm /root/outfile
fractel_readlog /root/outfile &
#fractel_routing_daemon /root/routing_log &
iptables -P FORWARD ACCEPT
iptables -F FORWARD
