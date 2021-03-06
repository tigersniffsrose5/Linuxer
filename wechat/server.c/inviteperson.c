#include "server.h"

void inviteperson(pack *recv)
{
    int flag = 0;
    char str[256];
    MYSQL_RES *res;   
    MYSQL_ROW row;

    if (mysql_real_query(&mysql, "select * from group_info", (unsigned long)strlen("select * from group_info"))) {  
        printf("mysql_real_query select failure!\n"); 
        exit(0);  
    }
    res = mysql_store_result(&mysql);
    if (NULL == res) {  
        printf("mysql_store_result failure!\n");  
        exit(0);  
    }
    while ((row = mysql_fetch_row(res))) {
        if ( strcmp(row[0], recv->recv_name)==0  ) {
            flag = 3;
            break;
        }
    }
    mysql_free_result(res);
    if ( flag != 3 ) {
        strcpy(recv->message, "no this group!");
        send(recv->send_fd, recv, sizeof(pack), 0);
        return ;
    }


    if (mysql_real_query(&mysql, "select * from nam_pass", (unsigned long)strlen("select * from nam_pass"))) {  
        printf("mysql_real_query select failure!\n"); 
        exit(0);  
    }
    res = mysql_store_result(&mysql);
    if (NULL == res) {  
        printf("mysql_store_result failure!\n");  
        exit(0);  
    }
    while ((row = mysql_fetch_row(res))) {
        if ( strcmp(recv->send_name, row[0]) == 0 ) {
            flag = 1;
            break;
        }
    }
    mysql_free_result(res);
    if ( flag != 1 ) {
        sprintf(recv->message, "no this person which named %s!", recv->send_name);
        send(recv->send_fd, recv, sizeof(pack), 0);
        return ;
    }


    if (mysql_real_query(&mysql, "select * from group_info", (unsigned long)strlen("select * from group_info"))) {  
        printf("mysql_real_query select failure!\n"); 
        exit(0);  
    }
    res = mysql_store_result(&mysql);
    if (NULL == res) {  
        printf("mysql_store_result failure!\n");  
        exit(0);  
    }
    while ((row = mysql_fetch_row(res))) {
        if ( strcmp(row[0], recv->recv_name)==0 && strcmp(row[1], recv->send_name) == 0 ) {
            flag = 2;
            break;
        }
    }
    if ( flag == 2 ) {
        strcpy(recv->message, "this person is already in the group!");
        send(recv->send_fd, recv, sizeof(pack), 0);
        return ;
    }
    mysql_free_result(res);
    sprintf(str, "insert into group_info values ('%s', '%s', %d)", recv->recv_name, recv->send_name, recv->ans);
    if (mysql_real_query(&mysql,str,strlen(str))) {
        printf("mysql_real_query insert failure!\n");
        exit(0);
    }
    strcpy(recv->message, "invite person success");
    send(recv->send_fd, recv, sizeof(pack), 0);
}


