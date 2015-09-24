@echo off
echo **** Client Usage: listening_port target_ip/target_network_name target_port folder_to_upload_files_from one/more_text_messages_to_pass ****
echo ***** Demonstrating all of the requirements with peer-peer communication and use of IP address *****
echo ***** Sending a binary file and a large pdf file *****
echo ***** 8484 127.0.0.1 8181 "./Test/sender1" "HI THERE!" *****

Debug\ClientExecutive.exe 8484 127.0.0.1 8181 /u Test

Debug\ClientExecutive.exe 8283 localhost 8181 /d