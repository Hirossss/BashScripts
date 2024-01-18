#!/bin/bash

trap 'handle_interrupt' INT

handle_interrupt() {
    echo -e "\n"
    read -p "Do you want to terminate the script now?(yes/no): " response

    if [[ "$response" = "yes" || "$response" = "y" || "$response" = "Y" ]]; then
        if [ -e <<PATH TO WRITEPID FILE>> ]; then
            pid=$(cat <<PATH TO WRITEPID FILE>>)
            echo "Terminating OpenVPN process with PID $pid"
            option_3
        else
            echo -e "\nYou haven't started any Ovpn connection"
            exit 1
        fi
        exit 1
    elif [[ "$response" = "no" || "$response" = "n" || "$response" = "N" ]]; then
        echo -e "\nContinuing the script..."
        # Return to the menu options loop
        menu_loop
    fi
}

check_tun0() {
    if [ $(ifconfig | grep tun0 | wc -l) -eq 0 ]; then
        echo -e "\nThe pop-up terminal was closed a while ago, the script is interrupted!"
        echo -e "\nYour session started at: "
        journalctl -u NetworkManager -e --lines=100 | grep -E 'manager: \(tun0\): new Tun' | tail -n 1 | awk '{print $1, $2, $3}'
        exit 0
    fi
}

option_3() {
    # Turn off the script
    check_tun0
    echo -e "\nSystem may ask you to authorize sudo power to send SIGTERM to the OVPN connection."
    sudo kill -SIGTERM $(cat <<PATH TO WRITEPID FILE>>)

    sleep 2

    declare -A month_dict
    month_dict["sty"]="01"
    month_dict["lut"]="02"
    month_dict["mar"]="03"
    month_dict["kwi"]="04"
    month_dict["maj"]="05"
    month_dict["cze"]="06"
    month_dict["lip"]="07"
    month_dict["sie"]="08"
    month_dict["wrz"]="09"
    month_dict["paź"]="10"
    month_dict["lis"]="11"
    month_dict["gru"]="12"

    # Get the relevant lines from journalctl
    lines=$(journalctl -u NetworkManager -e --lines=100 | grep -E 'manager: \(tun0\): new Tun device|activated -> unmanaged' | tail -n 2 | awk '{print $1, $2, $3}')

    # Extract the start and finish timestamps and reformat them using the dictionary
    start_month=${month_dict[$(echo "$lines" | head -n 1 | awk '{print $1}')]}
    start_day=$(echo "$lines" | head -n 1 | awk '{print $2}')
    start_time=$(echo "$lines" | head -n 1 | awk '{print $3}')
    start_timestamp="2023-${start_month}-${start_day} ${start_time}"

    finish_month=${month_dict[$(echo "$lines" | tail -n 1 | awk '{print $1}')]}
    finish_day=$(echo "$lines" | tail -n 1 | awk '{print $2}')
    finish_time=$(echo "$lines" | tail -n 1 | awk '{print $3}')
    finish_timestamp="2023-${finish_month}-${finish_day} ${finish_time}"

    # Convert timestamps to seconds in linux format
    start_time=$(date -d "$start_timestamp" +%s)
    finish_time=$(date -d "$finish_timestamp" +%s)

    # Calculate the time difference
    time_diff=$((finish_time - start_time))

    # Print the session stats
    echo -e "\nYour session stats:"
    echo "Start: $start_timestamp"
    echo "Finish: $finish_timestamp"
    echo "Time: $time_diff seconds"
    exit 0
}

menu_loop() {
    while true; do
        echo -e "\n\n*****Menu Options*****"
        echo "1. Log in via SSH provided in the script"
        echo "2. Check OpenVPN Tunnel Status"
        echo -e "3. Turn Off the Script"
        echo -e "4. Clear terminal\n"

        read -p "Enter your choice: " choice

        case $choice in
        1)
            # Log in via SSH
            check_tun0
            echo ""
            ssh <<YOUR SSH ADDRESS>>
            ;;
        2)
            # Check OpenVPN status
            check_tun0
            echo -e "\nYour tunnel status\n"
            ifconfig tun0
            ;;
        3)
            # Turn off the script
            option_3
            ;;
        4)
            clear
            ;;
        *)
            check_tun0
            echo -e "\nInvalid choice. Please select a valid option."
            ;;
        esac
    done
}

# We open the VPN connection in a new terminal so it doesn't disturb us.
gnome-terminal --title="OVPN CONNECTION" -- sudo openvpn --config <<PATH TO CONFIG FILE>> --writepid <<PATH TO WRITEPID FILE>>

echo "####################################################################################"
echo "Start the OVPN connection in a pop-up terminal."
echo "####################################################################################"

echo -e "\nTunnel waits for sudo access."
while [ $(ifconfig | grep tun0 | wc -l) -eq 0 ]; do
    echo -en "."
    sleep 1
done

# Start the menu loop
menu_loop

