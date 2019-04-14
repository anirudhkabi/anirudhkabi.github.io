iptables -A INPUT -p UDP -i wlan0 --dport 999 -j ACCEPT  # Allow control packets in
iptables -A OUTPUT -p UDP -o wlan0 --sport 999 -j ACCEPT # Allow control packets directly out
iptables -A OUTPUT -p ALL -o wlan0 -j QUEUE              # fractel outbound packets
iptables -A FORWARD -p ALL -o wlan0 -j QUEUE             # fractel outbound packets
