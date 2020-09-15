#!/usr/bin/env python
def comms():
    import socket
    import sys
    import ipaddress
    
    # Checks for correct arguments
    if len(sys.argv) < 3:
        print 'usage: client hostname(IP) port'
        exit()
    try:
        ip = ipaddress.ip_address(sys.argv[1].decode())
        print ('Using IP: %s, IP%s'%(ip,ip.version))
    except ValueError:
        print('address/netmask is invalid: %s' %sys.argv[1])
        exit()
    try:
        TCP_PORT = int(sys.argv[2])
    except ValueError:
        print('%s is not a valid Port'%sys.argv[2])
        exit()
    print 'Using TCP_PORT:',str(sys.argv[2])
    
    # Connects to socket:
    
    TCP_IP = sys.argv[1].decode()
    BUFFER_SIZE=1024
    
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.connect((TCP_IP,TCP_PORT))
    
    while True:
        MSG = raw_input("Message: \n")
        if MSG=='end':
            break
        s.send(MSG)
        RESP = s.recv(BUFFER_SIZE)
        print ('Answer: ',RESP)
    s.close()
    
    
comms()
exit()
