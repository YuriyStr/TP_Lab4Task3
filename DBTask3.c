#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

int autocommitOn = 1;

void showMenu()
{
    printf("1 - SELECT query\n");
    printf("2 - INSERT query\n");
    printf("3 - DELETE query\n");
    printf("4 - Output photo\n");
    printf (autocommitOn ? "5 - Disable autocommit\n" : "5 - Enable autocommit\n");
    printf("0 - Quit\n");}

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i]);
    }
    printf("\n");
    return 0;
}

/*int readPhoto (char *filename, FILE *fp)
{
    fp = fopen(filename, "rb");
    
    if (fp == NULL)
    {
        return 0;
    }
    
    fseek(fp, 0, SEEK_END);
    
    if (ferror(fp))
    {
        fclose(fp);
        return 0;
    }
    
    int flen = ftell(fp);
    
    if (flen == -1)
    {
        fclose(fp);
        return 0;
    }
    
    fseek(fp, 0, SEEK_SET);
    
    if (ferror(fp))
    {
        fclose(fp);
        return 0;
    }
    return flen;
}*/
char *params[] = {"id", "Surname", "BirthCountry"};



int main(int argc, char* argv[])
{
    sqlite3 *db;
    int rc;
    
    rc = sqlite3_open("Lab4_3.sqlite", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    
    for(;;)
    {
        char *zErrMsg = 0;
        char sql[20000];
        sqlite3_stmt *res;
        int code;
        int parameter;
        showMenu();
        scanf("%d", &code);
        
        switch(code)
        {
            case 1:
                printf("Select next option:\n");
                printf("1 - select all\n");
                printf("2 - select by id\n");
                printf("3 - select by surname\n");
                printf("4 - select by country\n");
                scanf("%d", &parameter);
                if (parameter == 1)
                {
                    strcpy(sql, "SELECT * from Employees");
                    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
                    if(rc != SQLITE_OK)
                    {
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }
                    
                }
                else if (parameter >= 2 && parameter <= 4)
                {
                    strcpy (sql, "SELECT * from Employees WHERE ");
                    strcat (sql, params[parameter - 2]);
                    strcat (sql, " = ?");
                    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
                    
                    if (rc == SQLITE_OK)
                    {
                        printf ("Enter parameter: ");
                        if (parameter == 2)
                        {
                            int id;
                            scanf("%d", &id);
                            sqlite3_bind_int(res, 1, id);
                        }
                        else
                        {
                            char text[256];
                            scanf("%s", text);
                            sqlite3_bind_text(res, 1, text, -1, SQLITE_TRANSIENT);
                        }
                        
                    } else
                    {
                        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                    }
                    
                    //int step;
                    while (sqlite3_step(res) == SQLITE_ROW)
                    {
                        int count = sqlite3_column_count(res);
                        for (int i = 0; i < count; i++)
                            printf ("%s = %s\n", sqlite3_column_name(res, i), sqlite3_column_text(res, i));
                        printf("\n");
                        
                    }
                }
                break;
            
            case 2:
            {
                char cont;
                int iter = 0;
                int idx;
                strcpy(sql, "");
                if (!autocommitOn)
                    strcat (sql, "BEGIN;\n");
                    //rc = sqlite3_exec(db, "BEGIN;\n", callback, NULL, &zErrMsg);
                do
                {
                    char buf[2048];
                    int id;
                   // FILE *fp;
                    int flen;
                    
                    strcat(sql, "INSERT into Employees (ID, Name, Surname, BirthDate, BirthPlace, BirthCountry, Address, Department, Position, WorksSince) VALUES (");
                    printf ("Enter ID: ");
                    scanf ("%d", &id);
                    sprintf (buf, "%d", id);
                    strcat(sql, buf);
                    strcat (sql, ", \'");
                    
                    printf ("Enter name: ");
                    scanf ("%s", buf);
                    strcat(sql, buf);
                    strcat (sql, "\', \'");
                    
                    printf ("Enter surname: ");
                    scanf ("%s", buf);
                    strcat(sql, buf);
                    strcat (sql, "\', \'");
                    
                    printf ("Enter birth date: ");
                    scanf ("%s", buf);
                    strcat(sql, buf);
                    strcat (sql, "\', \'");
                    
                    printf ("Enter birth place: ");
                    scanf ("%s", buf);
                    strcat(sql, buf);
                    strcat (sql, "\', \'");
                    
                    printf ("Enter birth country: ");
                    scanf ("%s", buf);
                    strcat(sql, buf);
                    strcat (sql, "\', ");
                    
                    printf ("Enter address (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    if (strcmp(buf, "-") == 0)
                        strcat (sql, "NULL, ");
                    else
                    {
                        strcat (sql, "\'");
                        strcat(sql, buf);
                        strcat (sql, "\', ");
                    }
                    
                    printf ("Enter department (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    if (strcmp(buf, "-") == 0)
                        strcat (sql, "NULL, ");
                    else
                    {
                        strcat (sql, "\'");
                        strcat(sql, buf);
                        strcat (sql, "\', ");
                    }

                    printf ("Enter position (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    if (strcmp(buf, "-") == 0)
                        strcat (sql, "NULL, ");
                    else
                    {
                        strcat (sql, "\'");
                        strcat(sql, buf);
                        strcat (sql, "\', ");
                    }

                    printf ("Enter date of starting working (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    if (strcmp(buf, "-") == 0)
                        strcat (sql, "NULL)\n;");
                    else
                    {
                        strcat (sql, "\'");
                        strcat(sql, buf);
                        strcat (sql, "\')\n;");
                    }

                    
                    /*char buf[2048];
                    char indexbuf[64];
                    int id;
                    // FILE *fp;
                    int flen;
                    
                    strcpy(sql, "INSERT into Employees (ID, Name, Surname, BirthDate, BirthPlace, BirthCountry, Address, Department, Position, WorksSince) VALUES (@id, @name, @surname, @birthdate, @birthplace, @birthcountry, @address, @dept, @pos, @workssince);\n");
                    
                    //sprintf(buf, "@id%d, @name%d, @surname%d, @birthdate%d, @birthplace%d, @birthcountry%d, @address%d, @dept%d, @pos%d, @workssince%d);\n", iter, iter, iter, iter, iter, iter, iter, iter, iter, iter);
                   // strcat (sql, buf);
                    
                    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
                    
                    if(rc == SQLITE_OK)
                    {
                    printf ("Enter ID: ");
                    scanf ("%d", &id);
                    //sprintf(indexbuf, "@id%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@id");
                    sqlite3_bind_int(res, idx, id);
                    
                    printf ("Enter name: ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@name%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@name");
                    sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    
                    printf ("Enter surname: ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@surname%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@surname");
                    sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    
                    printf ("Enter birth date: ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@birthdate%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@birthdate");
                    sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    
                    printf ("Enter birth place: ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@birthplace%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@birthplace");
                    sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    
                    printf ("Enter birth country: ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@birthcountry%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@birthcountry");
                    sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    
                    printf ("Enter address (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@address%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@address");
                    if (strcmp(buf, "-") == 0)
                        sqlite3_bind_null(res, idx);
                    else
                    {
                        sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    }
                    
                    printf ("Enter department (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@dept%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@dept");
                    if (strcmp(buf, "-") == 0)
                         sqlite3_bind_null(res, idx);
                    else
                    {
                        sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    }
                    
                    printf ("Enter position (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@pos%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@pos");
                    if (strcmp(buf, "-") == 0)
                         sqlite3_bind_null(res, idx);
                    else
                    {
                        sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    }
                    
                    printf ("Enter date of starting working (or \'-\' to set it as NULL): ");
                    scanf ("%s", buf);
                    //sprintf(indexbuf, "@workssince%d", i);
                    idx = sqlite3_bind_parameter_index(res, "@workssince");
                    if (strcmp(buf, "-") == 0)
                         sqlite3_bind_null(res, idx);
                    else
                    {
                        sqlite3_bind_text(res, idx, buf, -1, SQLITE_TRANSIENT);
                    }
                        sqlite3_step(res)

                    }*/
                    
                   /* printf ("Enter the file name of the the photo (or \'-\' if there is no photo): ");
                    scanf ("%s", buf);
                    if (strcmp (buf, "-") == 0 || !(flen = readPhoto(buf, fp)))
                    {
                        strcat (sql, "NULL);\n");
                        printf ("Press \'y\' to insert another item or any other key to stop insertions: ");
                        scanf(" %c", &cont);
                        getchar();
                        continue;
                    }
                    
                    strcat (sql, "?);\n");
                    char photo[flen + 1];
                    
                    int size = fread(photo, 1, flen, fp);
                    printf("hell");
                    rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
                    
                    if (rc == SQLITE_OK)
                    {
                        sqlite3_bind_blob(pStmt, 1, photo, size, SQLITE_STATIC);
                        
                        rc = sqlite3_step(pStmt);
                        
                        if (rc != SQLITE_DONE)
                        {
                            printf("execution failed: %s", sqlite3_errmsg(db));
                        }
                        
                        sqlite3_finalize(pStmt);
                    }
                    
                    fclose(fp);*/
                    
                    //iter++;
                    printf ("Press \'y\' to insert another item or any other key to stop insertions: ");
                    scanf(" %c", &cont);
                    getchar();
                    
                } while (cont == 'y');
                
                if (!autocommitOn)
                    strcat (sql, "COMMIT;\n");
                    //rc = sqlite3_exec(db, "COMMIT;\n", callback, NULL, &zErrMsg);
                
                rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
                if(rc != SQLITE_OK)
                {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }else
                {
                    fprintf(stdout, "Operation done successfully\n");
                }
                break;
            }
            
            case 3:
                printf("Select next option:\n");
                printf("1 - delete by id\n");
                printf("2 - delete by surname\n");
                printf("3 - delete by country\n");
                scanf("%d", &parameter);
                if (parameter >= 1 && parameter <= 3)
                {
                    strcpy (sql, "DELETE from Employees WHERE ");
                    strcat (sql, params[parameter - 1]);
                    strcat (sql, "= ?");
                    
                    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
                    if (rc == SQLITE_OK)
                    {
                        printf ("Enter parameter: ");
                        if (parameter == 1)
                        {
                            int id;
                            scanf("%d", &id);
                            sqlite3_bind_int(res, 1, id);
                        }
                        else
                        {
                            char text[256];
                            scanf("%s", text);
                            sqlite3_bind_text(res, 1, text, -1, SQLITE_TRANSIENT);
                        }
                        while (sqlite3_step(res) == SQLITE_ROW);
                        fprintf(stdout, "Operation done successfully\n");
                    } else
                    {
                        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                    }
                }
                break;
                
            case 4:
            {
                char filename[256];
                FILE *fp;
                int id;
                void *image;
                printf ("Enter blob file name: ");
                scanf("%s", filename);
                fp = fopen(filename, "wb");
                strcpy(sql, "SELECT Id, Photo from Employees WHERE id = ?");
                printf("Enter id: ");
                scanf ("%d", &id);
                rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
                
                if (rc == SQLITE_OK)
                {
                    sqlite3_bind_int(res, 1, id);
                    
                } else
                {
                    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
                    fclose(fp);
                    break;
                }
                
                
                int step = sqlite3_step(res);
                if (step == SQLITE_ROW)
                {
                    if (sqlite3_column_blob(res, 1) != NULL)
                        fwrite(sqlite3_column_blob(res, 1), 1, sqlite3_column_bytes(res, 1), fp);
                    printf("Operation done successfully\n");
                }
                fclose(fp);
                break;
            }
                
            case 5:
                autocommitOn = (autocommitOn ^ 1) & 1;
                break;
                
            case 0:
                return 0;
            default:
                printf("No such option\n");
        }
   
    }
    sqlite3_close(db);
    return 0;
}