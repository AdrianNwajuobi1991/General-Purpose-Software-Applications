from java.util import *
from javax.management import *
import javax.management.Attribute
from java.io import FileInputStream
import java.lang
import os
import string
import sys
 
print 'parsing properies file' 
propInputStream = FileInputStream("C:/Oracle/Middleware/Oracle_Home/oracle_common/common/bin/domain.properties")
configProps = Properties()
configProps.load(propInputStream)
print 'parsing properties file complete'

#usage()
#mode = configProps.get("mode")
usernames= configProps.get("usernames")
passwords=configProps.get("passwords")
ListOfManagedServers=configProps.get("servers")
NodeManagerHomeValue=configProps.get("NodeManagerHome")
domainName=configProps.get("domainName")
domainDir=configProps.get("domainDir")
ListenAddressValue=configProps.get("ListenAddressValue")
ListenPortValue=configProps.get("ListenPortValue")
AdminServerName = configProps.get("AdminServerName")
passwords_list = passwords.split("|")
managed_servers = ListOfManagedServers.split("|")
usernames_list = usernames.split("|")
AdminServerPassword = passwords_list[0]
NodeManagerPassowrd = passwords_list[1]
AdminServerUserName = usernames_list[0]
NodeManagerUsername = usernames_list[1]


if sys.argv[1] == "start":
	print 'mode is set to start'
	startNodeManager(verbose='true', NodeManagerHome=NodeManagerHomeValue, ListenPort=ListenPortValue, ListenAddress=ListenAddressValue)
	nmConnect(NodeManagerUsername,NodeManagerPassowrd,ListenAddressValue,ListenPortValue,domainName,domainDir)
	nmStart(AdminServerName)
	connect(AdminServerUserName,AdminServerPassword)
	for m_server in managed_servers:
		start(m_server, 'Server')
elif sys.argv[1] == "stop":
	print 'mode is set to stop'
	nmConnect(NodeManagerUsername,NodeManagerPassowrd,ListenAddressValue,ListenPortValue,domainName,domainDir)
	connect(AdminServerUserName,AdminServerPassword)
	for m_server in managed_servers:
		nmKill(m_server)
	nmKill(AdminServerName)
	stopNodeManager()
            
