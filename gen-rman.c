#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
cd c
cp /mnt/c/tmp/menu.c .
cc -o menu.exe menu.c
*/

/* RMAN script to validate the backup */
#define RMANSCRIPT01 "\
#!/bin/bash\n\
# RMAN script to validate the backup\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orcl \n\nValidate backup\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORAENV_ASK=NO\n\
export ORACLE_SID=$1\n\
. oraenv\n\
export ORAENV_ASK=YES\n\
rman << EOF\n\
connect target \"/ as sysbackup\"\n\
run {\n\
allocate channel c1 type 'sbt_tape' parms 'BLKSIZE=1048576, SBT_LIBRARY=/home/oracle/opt/dpsapps/rmanagent/lib/libddobk.so, ENV=(STORAGE_UNIT=ppdm-su, BACKUP_HOST=ppdm-ip, RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent)';\n\
backup validate database;\n\
release channel c1;\n\
}\n\
EOF\n\
exit;\n\
"

/* RMAN script to perform the backup */
#define RMANSCRIPT02 "\
#!/bin/bash\n\
# RMAN script to perform the full backup\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orcl \n\nPerform a full backup\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORACLE_SID=$1\n\
rman << EOF\n\
connect target \"/ as sysbackup\"\n\
run {\n\
configure device type sbt_tape parallelism 1;\n\
configure default device type to sbt_tape;\n\
configure channel device type 'sbt_tape' parms 'BLKSIZE=1048576, SBT_LIBRARY=/home/oracle/opt/dpsapps/rmanagent/lib/libddobk.so, ENV=(STORAGE_UNIT=ppdm-su, BACKUP_HOST=ppdm-ip, RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent)';\n\
backup incremental level 0 database include current controlfile format './ppdm-uuid/SS/Full_%d_%I_%u_%p';\n\
backup archivelog all delete input format './ppdm-uuid/SS/ArchLogs_%d_%I_%u_%p';\n\
}\n\
EOF\n\
exit;\n\
"

/* RMAN script to perform the backup - single channel */
#define RMANSCRIPT03 "\
#!/bin/bash\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orcl \n\nPerform a full backup - single channel\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORACLE_SID=$1\n\
rman << EOF\n\
connect target \"/ as sysbackup\"\n\
run {\n\
set CONTROLFILE AUTOBACKUP FORMAT FOR DEVICE TYPE 'SBT_TAPE' TO './ppdm-uuid/%F';\n\
configure controlfile autobackup on;\n\
allocate channel c0 DEVICE type 'SBT_TAPE' parms 'SBT_LIBRARY=/home/oracle/opt/dpsapps/rmanagent/lib/libddobk.so, ENV=(STORAGE_UNIT=ppdm-su, BACKUP_HOST=ppdm-ip, RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent)';\n\
backup database include current controlfile format './ppdm-uuid/Database_%d_%I_%u_%p';\n\
backup archivelog all delete input format './ppdm-uuid/ArchLogs_%d_%I_%u_%p';\n\
release channel c0;\n\
}\n\
EOF\n\
exit;\n\
"

#define RMANSCRIPT04 "\
#!/bin/bash\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orcl \n\n Run RMAN script \n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORACLE_SID=$1\n\
rman << EOF\n\
connect target \"/ as sysbackup\"\n\
run {\n\
\n\
\n\
\n\
}\n\
EOF\n\
exit;\n\
"

/* ddutil01.sh -  */
#define DDUTILSH01 "\
#!/bin/bash\n\n\
# ./ddutil-01.sh | grep \"Top Level\" | cut -d \":\" -f 2 | sed 's/ //g'\n\
# ./ddutil-01.sh | grep \"Storage Unit\" | cut -d \":\" -f 2 | sed 's/ //g'\n\
# ./ddutil-01.sh | grep \"DD hostname\" | cut -d \":\" -f 2 | sed 's/ //g'\n\
export RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent\n\
if [ \"$EUID\" -ne 0 ]\n\
  then echo \"Please run as root\"\n\
  exit\n\
fi\n\
echo \"List the version of ddutil\"\n\
/home/oracle/software/ddrman/ddutil -i\n\
echo \"Display the storage units and assigned DB\"\n\
/home/oracle/software/ddrman/ddutil -s\n\
/home/oracle/software/ddrman/ddutil -v rman\n\
/home/oracle/software/ddrman/ddutil -v system\n\
/home/oracle/software/ddrman/ddutil -v asset\n\
"

/* ddutil02.sh -  */
#define DDUTILSH02 "\
#!/bin/bash\n\n\
readonly numparms=2\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 DD-IP-addr storage-unit \nexample: $0 1.1.1.1 orcl \n\n List Oracle backups\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
if [ \"$EUID\" -ne 0 ]\n\
  then echo \"Please run as root\"\n\
  exit\n\
fi\n\
DD_IPADDR=$1\n\
DD_STORUNIT=$2\n\
export RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent\n\
/home/oracle/software/ddrman/ddutil -z $DD_IPADDR:$DD_STORUNIT -R -b \"-1 day\"\n\
/home/oracle/software/ddrman/ddutil -z $DD_IPADDR:$DD_STORUNIT -R | sort -k9\n\
\n\
"

/* ddutil03.sh -  */
#define DDUTILSH03 "\
#!/bin/bash\n\n\
export RMAN_AGENT_HOME=/home/oracle/opt/dpsapps/rmanagent\n\
$RMAN_AGENT_HOME/bin/ddutil -s\n\
"

/* ddutil04.sh -  */
#define DDUTILSH04 "\
#!/bin/bash\n\n\
\n\
"

/* watch4rman.sh */
#define SHELLSCRIPT01 "\
#!/bin/bash\n\n\
watch 'ps -ef | grep rman | grep -v grep ; echo \"\"; echo \"\"; df -h'\n\
"

/* cr-demodb.sh -  */
#define SHELLSCRIPT02 "\
#!/bin/bash\n\n\
\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orcl \n\nCreate a new database Oracle_SID\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORAENV_ASK=NO\n\
export ORACLE_SID=orc1\n\
. oraenv\n\
export ORAENV_ASK=YES\n\
export ORACLE_SID=$1\n\
export ORACLE_DATAFILE=/home/oracle/db/data\n\
export ORACLE_RECOVERY=/home/oracle/db/fra\n\
export ORACLE_HOME_LISTNER=$ORACLE_HOME\n\
dbca -silent -responseFile NO_VALUE -createDatabase -templateName General_Purpose.dbc -gdbname $ORACLE_SID -sid $ORACLE_SID -sysPassword Passw0rd9 -systemPassword Passw0rd9 -emConfiguration LOCAL -dbsnmpPassword Passw0rd9 -datafileDestination $ORACLE_DATAFILE -redoLogFileSize 2048 -recoveryAreaDestination $ORACLE_RECOVERY -storageType FS -characterSet AL32UTF8 -listeners $ORACLE_HOME_LISTNER -automaticMemoryManagement TRUE\n\
"

/* rm-demodb.sh -  */
#define SHELLSCRIPT03 "\
#!/bin/bash\n\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orc1 \n\nDelete an existing database Oracle_SID\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORAENV_ASK=NO\n\
export ORACLE_SID=$1\n\
. oraenv\n\
export ORAENV_ASK=YES\n\
dbca -silent -responseFile NO_VALUE -deleteDatabase -sourceDB $ORACLE_SID  -sysPassword Passw0rd9 -systemPassword Passw0rd9\n\
"

/* mod-tnsnames.sh -  */
#define SHELLSCRIPT04 "\
#!/bin/bash\n\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orc1 \n\nCreate a new database Oracle_SID\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORAENV_ASK=NO\n\
export ORACLE_SID=$1\n\
. oraenv\n\
export ORAENV_ASK=YES\n\
echo \"LISTENER_$1 =\" >> /opt/oracle/product/18c/dbhomeXE/network/admin/tnsnames.ora\n\
echo \"  (ADDRESS = (PROTOCOL = TCP) (HOST = oracle-01.demo.local) (PORT = 1521))\" >> /opt/oracle/product/18c/dbhomeXE/network/admin/tnsnames.ora\n\
lsnrctl reload\n\
lsnrctl status\n\
"

/* clean-sys.sh -  */
#define SHELLSCRIPT05 "\
#!/bin/bash\n\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orc1 \n\nCreate a new database Oracle_SID\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export SID=$1\n\
du -sh * | sort -rh | head -10\n\
rm -fr /opt/oracle/diag/rdbms/$SID/$SID/alert/log_*.xml\n\
rm -fr /opt/oracle/diag/rdbms/$SID/$SID/trace/*.tr?\n\
rm -fr /u01/app/oracle/software/database/stage\n\
rm -fr /u01/app/oracle/software/grid/stage\n\
> /opt/oracle/diag/rdbms/$SID/$SID/trace/alert_$SID.log\n\
"

/* archlog-demodb.sh */
#define SHELLSCRIPT06 "\
#!/bin/bash\n\n\
readonly numparms=1\n\
usage ()\n\
{\n\
  echo -e \"\n  usage: $0 Oracle_SID \nexample: $0 orc1 \n\nTurn on archivelog Oracle_SID database\n\" >&2; exit 1;\n\
}\n\
[ $# -ne $numparms ] && usage\n\
export ORAENV_ASK=NO\n\
export ORACLE_SID=$1\n\
. oraenv\n\
export ORAENV_ASK=YES\n\
sqlplus -S / as sysdba \"@/home/oracle/scripts/archlog.sql\"\n\
"
		
/* add-ddveora.sh */
#define SHELLSCRIPT07 "\
#!//usr/bin/expect\n\
set timeout -1\n\
spawn /home/oracle/scripts/add-ddveorakeys.sh\n\
expect \"Password: \"\n\
send -- \"Password123!\r\"\n\
expect eof\n\
\n\
"

/* add-ddveorakeys.sh */
#define SHELLSCRIPT08 "\
ssh sysadmin@ddve-01 adminaccess add ssh-keys < /home/oracle/.ssh/id_rsa.pub\n\
"

/* ask-ppdm.sh */
#define SHELLSCRIPT09 "\
echo \"ddboost storage-unit show\"\n\
echo \"Password = Password123!\"\n\
echo \"python /usr/local/brs/puppet/scripts/get_dd_mtree_credential.py sqlstu\"\n\
ssh admin@ppdm-01.demo.local\n\
"

/* ls-demodb.sh */
#define SHELLSCRIPT10 "\
#!/bin/bash\n\n\
\n\
"

/* archlog.sql */
#define SQLSCRIPT01 "\
shutdown immediate\n\
startup mount\n\
alter database archivelog\n\
alter database open\n\
exit\n\
"

/* rman-status.sql */
#define SQLSCRIPT02 "\
set linesize 200\n\
set pagesize 100\n\
/* backup sets */\n\
select RECID, STAMP, SET_STAMP, SET_COUNT, BACKUP_TYPE, CONTROLFILE_INCLUDED, INCREMENTAL_LEVEL, PIECES, START_TIME, COMPLETION_TIME, ELAPSED_SECONDS, BLOCK_SIZE, INPUT_FILE_SCAN_ONLY, KEEP, KEEP_UNTIL, KEEP_OPTIONS, MULTI_SECTION from V$BACKUP_SET order by RECID;\n\n\
/* Backup Pieces*/\n\
SELECT RECID, STAMP, SET_STAMP, SET_COUNT, PIECE#, COPY#, DEVICE_TYPE, HANDLE, COMMENTS, MEDIA, MEDIA_POOL, CONCUR, TAG, STATUS, START_TIME, COMPLETION_TIME, ELAPSED_SECONDS, DELETED, BYTES, IS_RECOVERY_DEST_FILE, RMAN_STATUS_RECID, RMAN_STATUS_STAMP, COMPRESSED, BACKED_BY_VSS, ENCRYPTED, BACKED_BY_OSB FROM V$BACKUP_PIECE ORDER BY SET_STAMP;\n\n\
/*  Files in Backup Sets */\n\
-- control files and datafiles in backup sets from the control file\n\
select RECID, STAMP, SET_STAMP, SET_COUNT BACKUPSET_COUNT, B.FILE#, d.NAME DATAFILE_NAME, B.CREATION_CHANGE#, B.CREATION_TIME, RESETLOGS_CHANGE#, RESETLOGS_TIME, INCREMENTAL_LEVEL, INCREMENTAL_CHANGE#, B.CHECKPOINT_CHANGE#, B.CHECKPOINT_TIME, ABSOLUTE_FUZZY_CHANGE#, MARKED_CORRUPT, MEDIA_CORRUPT, LOGICALLY_CORRUPT, DATAFILE_BLOCKS, B.BLOCKS, B.BLOCK_SIZE, OLDEST_OFFLINE_RANGE, COMPLETION_TIME, CONTROLFILE_TYPE, USED_CHANGE_TRACKING, BLOCKS_READ, USED_OPTIMIZATION, B.FOREIGN_DBID, B.PLUGGED_READONLY, B.PLUGIN_CHANGE#, B.PLUGIN_RESETLOGS_CHANGE#, B.PLUGIN_RESETLOGS_TIME, SECTION_SIZE, UNDO_OPTIMIZED from V$BACKUP_DATAFILE b, V$DATAFILE d where B.FILE# (+)= D.FILE# ORDER BY b.CREATION_TIME DESC;\n\n\
"

#define HELPMSG01 "\
Login as oracle, run ssh-keygen to generate the script\n\
Login as oracle, run scripts/add-ddveorakeys.sh to add the key\n\
Login as oracle, run dbf - /u01/app/oradata/ORCL/datafile\n\
Login as oracle, run ssh sysadmin@ddve-01 mtree list\n\
Login as oracle, run ssh sysadmin@ddve-01 user show list\n\
Login as oracle, run ssh sysadmin@ddve-01 ddboost storage-unit show\n\
Login as oracle, run ssh sysadmin@ddve-01 adminaccess show ssh-keys\n\
Login as oracle, run ssh sysadmin@ddve-01 adminaccess del ssh-keys 2\n\
du -sh * | sort -rh | head -10\n\
rm -fr /opt/oracle/diag/rdbms/orcl/orcl/alert/log_*.xml\n\
rm -fr /opt/oracle/diag/rdbms/orcl/orcl/trace/*.tr?\n\
tail -f /opt/oracle/diag/rdbms/orcl/orcl/trace/alert_orcl.log\n\
ls /opt/oracle/product/18c/dbhomeXE/dbs/snapcf_orcl*\n\
ls /opt/oracle/product/18c/dbhomeXE/dbs/spfileorcl.ora\n\
show parameter max_dump_file_size\n\
alter system set max_dump_file_size='200m';\n\
Login as oracle, to create scripts\n\
export ORACLE_HOME_DIR=/home/oracle\n\
export RMAN_AGENT_HOME=$ORACLE_HOME_DIR/opt/dpsapps/rmanagent\n\
export ORACLE_HOME=/opt/oracle/product/18c/dbhomeXE\n\
service agentService start\n\
service agentService status\n\
rm -fr /u01/app/oracle/software/database/stage\n\
rm -fr /u01/app/oracle/software/grid/stage\n\
tail -f /opt/dpsapps/agentsvc/logs/OpAgentSvc-07Jul20*.log\n\
ll $RMAN_AGENT_HOME/config/rman_agent.config\n\
ll $RMAN_AGENT_HOME/config/ddboost.config\n\
ll $RMAN_AGENT_HOME/config/rman_options.config\n\n\
"

void create_file(char *filename, char *buf) 
{
   FILE *fptr;

   fptr = fopen(filename, "w");
   fprintf(fptr, "%s", buf);
   fclose(fptr);
}

void process_option_a()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   
   printf("%s", (char *) HELPMSG01);
   
   printf("You have selected option A\n");   
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_b()
{
   #if __WIN32__
   system("cls");
   system("mkdir scripts");
   #else 
   system("clear");
   system("mkdir -p scripts");
   #endif
   printf("You have selected option B - create scripts directory, and creating script files\n");
   
   create_file("scripts/rman-01.sh", (char *) RMANSCRIPT01);
   create_file("scripts/rman-02.sh", (char *) RMANSCRIPT02);
   create_file("scripts/rman-03.sh", (char *) RMANSCRIPT03);
   create_file("scripts/rman-04.sh", (char *) RMANSCRIPT04);
   
   create_file("scripts/archlog.sql", (char *) SQLSCRIPT01);
   create_file("scripts/rman-status.sql", (char *) SQLSCRIPT02);
   
   create_file("scripts/watch4rman.sh", (char *) SHELLSCRIPT01);
   create_file("scripts/cr-demodb.sh", (char *) SHELLSCRIPT02);
   create_file("scripts/rm-demodb.sh", (char *) SHELLSCRIPT03);
   create_file("scripts/mod-tnsnames.sh", (char *) SHELLSCRIPT04);
   create_file("scripts/clean-sys.sh", (char *) SHELLSCRIPT05);
   create_file("scripts/archlog-demodb.sh", (char *) SHELLSCRIPT06);
   create_file("scripts/add-ddveora.sh", (char *) SHELLSCRIPT07);
   create_file("scripts/add-ddveorakeys.sh", (char *) SHELLSCRIPT08);
   create_file("scripts/ask-ppdm.sh", (char *) SHELLSCRIPT09);
   create_file("scripts/ls-demodb.sh", (char *) SHELLSCRIPT10);
     
   create_file("scripts/ddutil-01.sh", DDUTILSH01);
   create_file("scripts/ddutil-02.sh", DDUTILSH02);
   create_file("scripts/ddutil-03.sh", DDUTILSH03);
   create_file("scripts/ddutil-04.sh", DDUTILSH04);
			
   printf("Hello world\n");

   #if __linux__
   system("id");
   system("chmod 755 scripts/*.sh");
   #endif

   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_c()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option C - ssh sysadmin add ddboost and create storage-unit\n");
   #if __linux__
   system("ssh sysadmin@ddve-01 user add ddboost role admin password Password123!");
   system("ssh sysadmin@ddve-01 ddboost storage-unit create sqlstoreunit user ddboost");
   #endif
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_d()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   #if __linux__
   printf("--> yum -y install gcc expect lsof\n");
   system("yum -y install gcc expect lsof");
   /*
   printf("--> rm -fr /u01/app/oracle/software/database/stage\n");
   system("rm -fr /u01/app/oracle/software/database/stage");
   printf("--> rm -fr /u01/app/oracle/software/grid/stage\n");
   system("rm -fr /u01/app/oracle/software/grid/stage");
   */
   #endif
   printf("You have selected option D - install expect as root\n");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_e()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   
   printf("--> mtree list\n");
   system("ssh sysadmin@ddve-01 mtree list");
   printf("\n--> ddboost storage-unit show\n");
   system("ssh sysadmin@ddve-01 ddboost storage-unit show");
   printf("\n--> user show list\n");
   system("ssh sysadmin@ddve-01 user show list");
   printf("You have selected option E\n");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_f()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option F - turn archivelog demodb\n");
   printf("--> /home/oracle/scripts/archlog-demodb.sh demodb\n");
   system("/home/oracle/scripts/archlog-demodb.sh demodb");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_g()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   
   printf("You have selected option G - login as oracle and create demodb database\n");
   printf("- You can monitor the logs in /u01/app/oracle/cfgtoollogs/dbca/demodb/trace.log\n");
   printf("- To check the DB activities -  /u01/app/oracle/diag/rdbms/demodb/demodb/trace/alert_demodb.log\n\n");
   printf("--> /home/oracle/scripts/cr-demodb.sh demodb\n");
   system("/home/oracle/scripts/cr-demodb.sh demodb");
   printf("--> /home/oracle/scripts/mod-tnsnames.sh demodb\n");
   system("/home/oracle/scripts/mod-tnsnames.sh demodb");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_h()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   
   printf("You have selected option H - login as oracle and destroy demodb database\n");
   printf("--> /home/oracle/scripts/rm-demodb.sh demodb\n");
   system("/home/oracle/scripts/rm-demodb.sh demodb");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_j()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option J - customise Oracle RMAN scripts\n");

   char stor_unit[30], host_ip[30], ppdm_uuid[30];

   printf("Enter the storage_unit (oracle-ppdm-01-ca024) : ");
   scanf("%s", stor_unit);
   printf("Enter the PPDM UUID (PLCTLP-): ");
   scanf("%s", ppdm_uuid);
   printf("Enter the PPDM IP address (192.168.1.30): ");
   scanf("%s", host_ip);

   char cmd_line[80];
   
   /* part of <stdio.h>
   #if __WIN32__
   #elif __linux__
   #elif __unix__   
   */
   #if __APPLE__
   sprintf(cmd_line, "sed -i '' 's/ppdm-uuid/%s/g' scripts/rman*.sh", ppdm_uuid);
   system(cmd_line);
   sprintf(cmd_line, "sed -i '' 's/ppdm-su/%s/g' scripts/rman*.sh", stor_unit);
   system(cmd_line);   
   sprintf(cmd_line, "sed -i '' 's/ppdm-ip/%s/g' scripts/rman*.sh", host_ip);
   system(cmd_line);
   #else 
   sprintf(cmd_line, "sed -i 's/ppdm-uuid/%s/g' scripts/rman*.sh", ppdm_uuid);
   system(cmd_line);   
   sprintf(cmd_line, "sed -i 's/ppdm-su/%s/g' scripts/rman*.sh", stor_unit);
   system(cmd_line);      
   sprintf(cmd_line, "sed -i 's/ppdm-ip/%s/g' scripts/rman*.sh", host_ip);
   system(cmd_line);   
   #endif

   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_k()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option K - clean Oracle space\n");
   
   printf("--> rm -fr /opt/oracle/diag/rdbms/orcl/orcl/alert/log_*.xml\n");
   system("rm -fr /opt/oracle/diag/rdbms/orcl/orcl/alert/log_*.xml");
   system("rm -fr /opt/oracle/diag/rdbms/orcl/orcl/trace/*.tr?");
   system("cat /dev/null > /opt/oracle/diag/rdbms/orcl/orcl/trace/alert_orcl1.log");
   printf("--> rm -fr /opt/oracle/diag/rdbms/demodb/demodb/alert/log_*.xml\n");
   system("rm -fr /opt/oracle/diag/rdbms/demodb/demodb/alert/log_*.xml");
   system("rm -fr /opt/oracle/diag/rdbms/demodb/demodb/trace/*.tr?");
   system("cat /dev/null > /opt/oracle/diag/rdbms/demodb/demodb/trace/alert_demodb.log");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_l()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option L - ssh admin@ppdm-01.demo.local\n");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_m()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option M\n");
   printf("Enter to return to main menu\n");
   getchar();
}

void process_option_n()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("You have selected option N\n");
   printf("Enter to return to main menu\n");
   getchar();
}

void list_menu_options()
{
   #if __WIN32__
   system("cls");
   #else 
   system("clear");
   #endif
   printf("\nMenu Options\n");
   printf("------------------------------------------------------\n");
   printf("a: Option A - HELP\n");
   printf("b: Option B - create scripts\n");
   printf("c: Option C - setup ddboost\n");   
   printf("d: Option D - yum -y install expect lsof [ root ]\n");
   printf("e: Option E - ssh sysadmin@ddve-01 \n");
   printf("f: Option F - turn archivelog demodb\n");
   printf("g: Option G - create demodb database\n");
   printf("h: Option H - destroy demodb database\n");
   printf("j: Option J - customise Oracle RMAN scripts\n");
   printf("k: Option K - clean Oracle space\n");
   printf("l: Option L - ssh admin@ppdm-01.demo.local\n");
   printf("m: Option M\n");   
   printf("n: Option N\n");      
   printf("z: Save and quit\n");
   printf("\n\nPlease enter a choice (a, b, c, d, e, f, g, h, j, k, l, m, n or z) ---> ");
}

void process_menu()
{
   char ch;

   do {
      list_menu_options();
      ch = tolower(getchar());  // read a char, convert to lower case
      getchar();                // clear the newline char
      switch(ch) {
         case 'a':
            process_option_a();
            break;
         case 'b':
            process_option_b();
            break;
         case 'c':
            process_option_c();
            break;
         case 'd':
            process_option_d();
            break;
         case 'e':
            process_option_e();
            break;                        
         case 'f':
            process_option_f();
            break;
         case 'g':
            process_option_g();
            break;
         case 'h':
            process_option_h();
            break;
         case 'j':
            process_option_j();
            break;
         case 'k':
            process_option_k();
            break;
         case 'l':
            process_option_l();
            break;
         case 'm':
            process_option_m();
            break;
         case 'z':
            break;
         default:
            printf("\nError: Invalid Input.  Please try again...");
            break;
       }
    } while (ch != 'z');
}


int main()
{
   process_menu();
  return(0);
}