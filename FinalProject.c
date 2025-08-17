#include<stdio.h>
#include "Project.h"
#include<windows.h>
#include <conio.h>
#include<string.h>
#define MAX_BOOKS 100
#define MAX_LEN 256
struct Book {
    char ISBN[30];
    char title[100];
    char author[100];
    char publish_year[10];
    char no_copies[10];
};
void gotoxy(int x,int y);
void draw_text_box(int x, int y, int width);
void draw_box();
void main_menu();
void student_menu();
void admin_menu();
void student_login();
void admin_login();
void check_add_book();
struct Book add_book();
void print_books();
void return_book();
int contains_ignore_case(const char *text, const char *searchTerm);
void search_books(struct Book library[], int bookCount);
void delete_book();
int load_books_from_file(struct Book library[]);
void borrow_book();
char *menu_login[2] = { "-1) Login as Student            ", "-2) Login as Admin        "};
char *menu_student[4] = { "-1) Borrow Book        ", "-2) Search for a Book         ","-3) Return a book         ","-4) Logout           "};
char *menu_admin[4] = { "-1) Add Book        ", "-2) Search for a Book         ","-3) Delete book         ","-4) Logout           "};
int pos=1;
char ch;

/////////////////////////////////////////MAIN FUNCTION BOUNDARIES
void main(void){
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    draw_box();
    main_menu();

        switch(pos){                    //main switch case, 1 for student page , 2 for admin page
        case 1 :
                system("cls");
                student_login();
                break;

        case 2:
                system("cls");
                admin_login();
                break;
            }


}
/////////////////////////////////////////MAIN FUNCTION BOUNDARIES
void borrow_book() {
    system("color 0F");
    char isbn_to_borrow[30];
    struct Book books[MAX_BOOKS];
    int count = load_books_from_file(books);
    if (count == 0) return;
    system("cls");
    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("Enter ISBN to borrow: ");
    gotoxy(12, 6);
    fgets(isbn_to_borrow, sizeof(isbn_to_borrow), stdin);
    isbn_to_borrow[strcspn(isbn_to_borrow, "\n")] = '\0';
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].ISBN, isbn_to_borrow) == 0) {
            int copies = atoi(books[i].no_copies);
            if (copies > 0) {
                copies--;
                sprintf(books[i].no_copies, "%d", copies);
                found = 1;
            } else {
                gotoxy(15, 10);
                printf("No copies available for this book.\n");
                printf("\nPress any key to continue...");
                getch();
                return;
            }
            break;
        }
    }
    if (!found) {
        gotoxy(15, 10);
        printf("Book not found.\n");
        printf("\nPress any key to continue...");
        getch();
        return;
    }
    FILE *fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "w");
    if (!fp) {
        printf("Error: Cannot write to books file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s\n",
                books[i].ISBN, books[i].title, books[i].author,
                books[i].publish_year, books[i].no_copies);
    }
    fclose(fp);
    gotoxy(15, 10);
    printf("Book borrowed successfully.\n");
    printf("\nPress any key to continue...");
    getch();
}
void delete_book() {
    system("color 0F");
    char isbn_to_delete[30];
    struct Book books[MAX_BOOKS];
    int count = load_books_from_file(books);
    if (count == 0) return;
    system("cls");
    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("Enter ISBN to delete: ");
    gotoxy(12, 6);
    fgets(isbn_to_delete, sizeof(isbn_to_delete), stdin);
    isbn_to_delete[strcspn(isbn_to_delete, "\n")] = '\0';
    int found = 0;
    int new_count = 0;
    struct Book temp_books[MAX_BOOKS];
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].ISBN, isbn_to_delete) != 0) {
            temp_books[new_count++] = books[i];
        } else {
            found = 1;
        }
    }
    if (!found) {
        gotoxy(15, 10);
        printf("Book not found.\n");
        printf("\nPress any key to continue...");
        getch();
        return;
    }
    FILE *fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "w");
    if (!fp) {
        printf("Error: Cannot write to books file.\n");
        return;
    }
    for (int i = 0; i < new_count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s\n",
                temp_books[i].ISBN, temp_books[i].title, temp_books[i].author,
                temp_books[i].publish_year, temp_books[i].no_copies);
    }
    fclose(fp);
    gotoxy(15, 10);
    printf("Book deleted successfully.\n");
    printf("\nPress any key to continue...");
    getch();
}
int load_books_from_file(struct Book library[]){
    FILE *fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "r");
    if (!fp) {
        printf("Error: Could not open file.\n");
        return 0;
    }

    char line[MAX_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && count < MAX_BOOKS)
    {
        line[strcspn(line, "\n")] = '\0'; // remove newline
        char *token = strtok(line, ",");

        if (token) strncpy(library[count].ISBN, token, sizeof(library[count].ISBN));
        token = strtok(NULL, ",");
        if (token) strncpy(library[count].title, token, sizeof(library[count].title));
        token = strtok(NULL, ",");
        if (token) strncpy(library[count].author, token, sizeof(library[count].author));
        token = strtok(NULL, ",");
        if (token) strncpy(library[count].publish_year, token, sizeof(library[count].publish_year));
        token = strtok(NULL, ",");
        if (token) strncpy(library[count].no_copies, token, sizeof(library[count].no_copies));

        count++;
    }

    fclose(fp);
    return count;
}
void search_books(struct Book library[], int bookCount) {
    system("cls");
    system("color 0F");
    char searchTerm[MAX_LEN];
    int found = 0;
    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("Enter a title or author of the book to search: ");
    gotoxy(12, 6);
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    printf("Search Results:\n");

    for (int i = 0; i < bookCount; i++)
    {
        if (contains_ignore_case(library[i].title, searchTerm) ||
            contains_ignore_case(library[i].author, searchTerm))
        {

            printf("Title       : %s\n", library[i].title);
            printf("Author      : %s\n", library[i].author);
            printf("ISBN        : %s\n", library[i].ISBN);
            printf("Year        : %s\n", library[i].publish_year);
            printf("Copies Left : %s\n", library[i].no_copies);
            printf("----------------------------\n");
            found = 1;
        }
    }

    if (!found)
        printf("No books found matching : '%s'.\n", searchTerm);

}
int contains_ignore_case(const char *text, const char *searchTerm) {
    char lowerText[MAX_LEN], lowerSearch[MAX_LEN];
    int i;

    for (i = 0; text[i] && i < MAX_LEN - 1; i++)
        lowerText[i] = tolower(text[i]);
        lowerText[i] = '\0';

    for (i = 0; searchTerm[i] && i < MAX_LEN - 1; i++)
        lowerSearch[i] = tolower(searchTerm[i]);
        lowerSearch[i] = '\0';

    return strstr(lowerText, lowerSearch) != NULL;
}
void return_book() {
    system("color 0F");
    char isbn_to_return[30];
    struct Book books[MAX_BOOKS];
    int count = 0;
    char line[MAX_LEN];

    FILE *fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "r");

    if (!fp) {
        printf("Error: Cannot open books file.\n");
        return;
    }

    // Read books into memory
    while (fgets(line, MAX_LEN, fp) && count < MAX_BOOKS) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token) strcpy(books[count].ISBN, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].title, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].author, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].publish_year, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].no_copies, token);

        count++;
    }

    fclose(fp);

    // Ask user to enter ISBN of book to return
    system("cls");
    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("Enter ISBN to return: ");
    gotoxy(12, 6);
    fgets(isbn_to_return, sizeof(isbn_to_return), stdin);
    isbn_to_return[strcspn(isbn_to_return, "\n")] = '\0';

    // Search for the book
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].ISBN, isbn_to_return) == 0) {
            int copies = atoi(books[i].no_copies);
            copies++;
            sprintf(books[i].no_copies, "%d", copies);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
        return;
    }

    // Write updated list back to file
    fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "w");
    if (!fp) {
        printf("Error: Cannot write to books file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s\n",
            books[i].ISBN,
            books[i].title,
            books[i].author,
            books[i].publish_year,
            books[i].no_copies);
    }

    fclose(fp);
    gotoxy(15, 10);
    printf("Book returned successfully.\n");
    printf("\nPress any key to continue...");
    getch();
}
void print_books(){
FILE *fp;
struct Book books[MAX_BOOKS];
int count = 0;
char line[MAX_LEN];

fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "r");

if (!fp) {
        printf("Error: Cannot open books file.\n");
        return;
}

while (fgets(line, MAX_LEN, fp) && count < MAX_BOOKS) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token) strcpy(books[count].ISBN, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].title, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].author, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].publish_year, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].no_copies, token);

        count++;
    }

    fclose(fp);

for (int i = 0; i < count; i++) {
        printf("%-15s %-30s %-25s %-15s %-10s\n",
               books[i].ISBN,
               books[i].title,
               books[i].author,
               books[i].publish_year,
               books[i].no_copies);
    }
}
struct Book add_book(){
    struct Book newBook;

                system("cls");
                draw_text_box(10, 5, 30);
                gotoxy(8, 2);
                printf("Enter ISBN: ");
                gotoxy(12, 6);
                fgets(newBook.ISBN, sizeof(newBook.ISBN), stdin);
                newBook.ISBN[strcspn(newBook.ISBN, "\n")] = '\0';


                system("cls");
                draw_text_box(10, 5, 30);
                gotoxy(8, 2);
                printf("Enter Title: ");
                gotoxy(12, 6);
                fgets(newBook.title, sizeof(newBook.title), stdin);
                newBook.title[strcspn(newBook.title, "\n")] = '\0';


                system("cls");
                draw_text_box(10, 5, 30);
                gotoxy(8, 2);
                printf("Enter Author: ");
                gotoxy(12, 6);
                fgets(newBook.author, sizeof(newBook.author), stdin);
                newBook.author[strcspn(newBook.author, "\n")] = '\0';


                system("cls");
                draw_text_box(10, 5, 30);
                gotoxy(8, 2);
                printf("Enter Publish Year: ");
                gotoxy(12, 6);
                fgets(newBook.publish_year, sizeof(newBook.publish_year), stdin);
                newBook.publish_year[strcspn(newBook.publish_year, "\n")] = '\0';


                system("cls");
                draw_text_box(10, 5, 30);
                gotoxy(8, 2);
                printf("Enter Number of Copies: ");
                gotoxy(12, 6);
                fgets(newBook.no_copies, sizeof(newBook.no_copies), stdin);
                newBook.no_copies[strcspn(newBook.no_copies, "\n")] = '\0';


    return newBook;

}
void check_add_book(){
system("color 0F");
FILE *fp;
char line[MAX_LEN];
int count=0;
struct Book books[MAX_BOOKS];

fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "r");

    if (!fp) {
        printf("Error: Cannot open admin file.\n");
    }

    while (fgets(line, MAX_LEN, fp) && count < MAX_BOOKS) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");

        if (token) strcpy(books[count].ISBN, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].title, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].author, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].publish_year, token);
        token = strtok(NULL, ",");
        if (token) strcpy(books[count].no_copies, token);

        count++;

    }

     fclose(fp);


    struct Book newBook=add_book();
    int found = 0;

    for (int i = 0; i < count; i++) {
    printf("Book %d: %s by %s\n", i+1, books[i].title, books[i].author);
    }
    for(int i=0;i<count;i++)
    {

            if(strcmp(books[i].title, newBook.title) == 0)
        {
                system("cls");
                int no_copies = atoi(books[i].no_copies);
                no_copies++;
                printf("The book you are adding is already in the library \n");
                printf("The number of copies of this book now are %d",no_copies);
                found = 1;
                sprintf(books[i].no_copies, "%d", no_copies);
                printf("\nPress any key to continue...");
                getch();  // wait so the user can read the message
                break;
        }
    }

            if (!found && count < MAX_BOOKS) {
                books[count++] = newBook;
                system("cls");
                printf("New book added to the library.\n");

                printf("\nPress any key to continue...");
                getch();  // wait so the user can read the message
            }


            fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\books.txt", "a");
            if (!fp) {
                printf("Error: Cannot write to books file.\n");
                return;
            }

fprintf(fp, "%s,%s,%s,%s,%s\n",
        newBook.ISBN,
        newBook.title,
        newBook.author,
        newBook.publish_year,
        newBook.no_copies);

    fclose(fp);
}
void main_menu(){
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    do{
            SetConsoleTextAttribute(Console,15);
            gotoxy(45,15);
            printf(" %s",menu_login[0]);
            gotoxy(45,19);
            printf(" %s",menu_login[1]);
    switch(pos){
    case 1:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,15);
            printf(" %s",menu_login[0]);
            break;
    case 2:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,19);
            printf(" %s",menu_login[1]);
            break;
    }


        ch=getch();
        if(ch==-32)
            ch=getch();
        if(ch==72)
        {
            pos--;
            if(pos==0)
                pos=2;
        }
        if(ch==80){
            pos++;
            if(pos==3)
                pos=1;
        }
 }while(ch!=13);

gotoxy(45, 25);
SetConsoleTextAttribute(Console, 15);
printf("Your selection is %s \n", menu_login[pos-1]);

}
void student_menu(){
pos=1;
HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
     do{
            SetConsoleTextAttribute(Console,15);
            gotoxy(45,15);
            printf(" %s",menu_student[0]);
            gotoxy(45,17);
            printf(" %s",menu_student[1]);
            gotoxy(45,19);
            printf(" %s",menu_student[2]);
            gotoxy(45,21);
            printf(" %s",menu_student[3]);
    switch(pos){
    case 1:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,15);
            printf(" %s",menu_student[0]);
            break;
    case 2:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,17);
            printf(" %s",menu_student[1]);
            break;

    case 3:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,19);
            printf(" %s",menu_student[2]);
            break;
    case 4:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,21);
            printf(" %s",menu_student[3]);
            break;
    }

        ch=getch();
        if(ch==-32)
            ch=getch();
        if(ch==72)
        {
            pos--;
            if(pos==0)
                pos=4;
        }
        if(ch==80){
            pos++;
            if(pos==5)
                pos=1;

            }
     }while(ch!=13);

    switch(pos){

        case 1 :
                    borrow_book();
                    student_menu();
                    break;
        case 2 :
                    struct Book library[MAX_BOOKS];
                    int totalBooks = load_books_from_file(library);
                    if (totalBooks > 0)
                    search_books(library, totalBooks);
                    student_menu();
                    break;
        case 3 :
                    return_book();
                    student_menu();
                    break;
        case 4 :
                    system("cls");
                    system("color 0F");
                    student_login();
                    break;
            }
}
void admin_menu(){
pos=1;
HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
do{
            SetConsoleTextAttribute(Console,15);
            gotoxy(45,15);
            printf(" %s",menu_admin[0]);
            gotoxy(45,17);
            printf(" %s",menu_admin[1]);
            gotoxy(45,19);
            printf(" %s",menu_admin[2]);
            gotoxy(45,21);
            printf(" %s",menu_admin[3]);
    switch(pos){
    case 1:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,15);
            printf(" %s",menu_admin[0]);
            break;
    case 2:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,17);
            printf(" %s",menu_admin[1]);
            break;

    case 3:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,19);
            printf(" %s",menu_admin[2]);
            break;
    case 4:
            SetConsoleTextAttribute(Console,240);
            gotoxy(45,21);
            printf(" %s",menu_admin[3]);
            break;
    }

        ch=getch();
        if(ch==-32)
            ch=getch();
        if(ch==72)
        {
            pos--;
            if(pos==0)
                pos=4;
        }
        if(ch==80){
            pos++;
            if(pos==5)
                pos=1;

            }
     }while(ch!=13);

    switch(pos){

        case 1 :
                    struct Book books[100];
                    check_add_book();
                    admin_menu();
                    break;
        case 2 :
                    struct Book library[MAX_BOOKS];
                    int totalBooks = load_books_from_file(library);
                    if (totalBooks > 0)
                    search_books(library, totalBooks);
                    admin_menu();
                    break;
        case 3 :
                    delete_book();
                    admin_menu();
                    break;
        case 4 :
                    admin_login();
                    break;



    }



}
void gotoxy(int x,int y){
COORD coord ={0,0};
coord.X=x;
coord.Y=y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void draw_box(){
    gotoxy(44,14);
    printf("%c",201);

    for(int i=0;i<33;i++)
        printf("%c",205);

    printf("%c",187);
    gotoxy(44,15);

    for(int i=0;i<10;i++)
    {
     printf("%c",186);
     gotoxy(44,15+i);
    }
    gotoxy(78,15);

    for(int i=0;i<10;i++)
    {
     printf("%c",186);
     gotoxy(78,15+i);
    }

    gotoxy(45,24);
    for(int i=0;i<33;i++)
    printf("%c",205);

     gotoxy(44,24);
    printf("%c",200);

    gotoxy(78,24);
    printf("%c",188);

}
void draw_text_box(int x, int y, int width) {
    gotoxy(x, y);
    printf("%c", 201);  // ┌
    for(int i = 0; i < width; i++) printf("%c", 205);  // ─
    printf("%c", 187);  // ┐

    gotoxy(x, y+1);
    printf("%c", 186);  // │
    for(int i = 0; i < width; i++) printf(" ");
    printf("%c", 186);  // │

    gotoxy(x, y+2);
    printf("%c", 200);  // └
    for(int i = 0; i < width; i++) printf("%c", 205);  // ─
    printf("%c", 188);  // ┘
}
void student_login(){
    FILE *fp;
    char file_username[MAX_LEN];
    char file_password[MAX_LEN];
    int student_pass;
    int student_user_length;
    int student_pass_length;
    char student_username[MAX_LEN];
    char student_password[MAX_LEN];
    int i=0;
    int found = 0;
draw_text_box(10,5,30);
gotoxy(8, 2);
printf("    Enter Username\n");
gotoxy(12, 6);
gets(student_username);
student_user_length=strlen(student_username);
system("cls");
draw_text_box(10,5,30);
gotoxy(8, 2);
printf("    Enter Password\n");
gotoxy(12, 6);
 while (1) {
        ch = getch();

        if (ch == 13)
            break;
        if (ch == 8) { //backspace "delete character"
            if (i > 0) {
                i--;
                student_password[i] = '\0';
                printf("\b \b");
            }
        }
        else if (ch >= '0' && ch <= '9') {
            if (i < sizeof(student_password) - 1) {
                student_password[i++] = ch;
                student_password[i] = '\0';
                printf("*");
            }
        }
    }

    student_pass = atoi(student_password);         // convert to int
    student_pass_length = i;                       // number of digits entered
    //////////////////////////////////
    fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\students.txt", "r");
    if (!fp) {
        printf("Error: Cannot open file.\n");
    }

    while (fgets(file_username, MAX_LEN, fp) && fgets(file_password, MAX_LEN, fp)) {
        file_username[strcspn(file_username, "\n")] = '\0';
        file_password[strcspn(file_password, "\n")] = '\0';

        if (strcmp(student_username, file_username) == 0 &&
            strcmp(student_password, file_password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
            gotoxy(12, 8);
        printf(" Login successful! Welcome, %s.\n", student_username);
        draw_box();
        student_menu();
    } else {
        gotoxy(12, 8);
        printf(" Incorrect username or password.\n");
    }


    //////////////////////////////////
}
void admin_login() {
    FILE *fp;
    char file_username[MAX_LEN];
    char file_password[MAX_LEN];
    int admin_pass;
    int admin_user_length;
    int admin_pass_length;
    char admin_username[MAX_LEN];
    char admin_password[MAX_LEN];
    char ch;
    int i = 0;
    int found = 0;

    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("    Enter Username\n");
    gotoxy(12, 6);
    gets(admin_username);  // Warning: unsafe, consider using fgets() instead
    admin_user_length = strlen(admin_username);

    system("cls");
    draw_text_box(10, 5, 30);
    gotoxy(8, 2);
    printf("    Enter Password\n");
    gotoxy(12, 6);

    while (1) {
        ch = getch();  // get character without echo

        if (ch == 13)  // Enter key
            break;
        if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                admin_password[i] = '\0';
                printf("\b \b");  // erase last '*'
            }
        }
        else if (ch >= '0' && ch <= '9') {
            if (i < sizeof(admin_password) - 1) {
                admin_password[i++] = ch;
                admin_password[i] = '\0';
                printf("*");
            }
        }
    }

    admin_pass = atoi(admin_password);       // convert to int if needed
    admin_pass_length = i;                   // number of digits entered

    ////////////////////// FILE READ //////////////////////
    fp = fopen("C:\\Users\\user\\Desktop\\Intro to Programming using C\\Project\\files\\admin.txt", "r");
    if (!fp) {
        printf("Error: Cannot open admin file.\n");
        return;
    }

    while (fgets(file_username, MAX_LEN, fp) && fgets(file_password, MAX_LEN, fp)) {
        file_username[strcspn(file_username, "\n")] = '\0';
        file_password[strcspn(file_password, "\n")] = '\0';

        if (strcmp(admin_username, file_username) == 0 &&
            strcmp(admin_password, file_password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
        gotoxy(12, 8);
        printf(" Login successful! Welcome, %s.\n", admin_username);
        draw_box();
        admin_menu();
    } else {
        gotoxy(12, 8);
        printf(" Incorrect admin username or password.\n");
    }
}

