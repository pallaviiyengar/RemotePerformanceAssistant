@echo off
echo **** Client Usage: listening_port target_ip/target_network_name target_port folder_to_upload_files_from one/more_text_messages_to_pass ****
echo ***** Demonstrating all of the requirements with peer-peer communication and use of network name *****
echo ***** Sending a text file, an image file and more than one message*****
echo ***** 8282 localhost 8181 "./Test/sender2" "HOW YOU DOING!" "HEY THERE!" *****
Debug\ClientExecutive.exe 8283 localhost 8181 /d

