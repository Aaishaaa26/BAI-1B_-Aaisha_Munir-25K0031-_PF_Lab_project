#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define emp_f "employees.txt"
#define attend_f "attendance.txt"
#define pay_f "payroll.txt"
#define leave_f "leave.txt"
#define job_f "jobs.txt"
#define app_f "applications.txt"
#define perform_f "performance.txt"
#define train_f "training.txt"
#define backup_f "backup.txt"

// Employee Structure
struct Employee
{
    int id;
    char name[100];
    float salary;
    char contact[20];
};
// THIS IS STRUCT FOR PERFORMANCE
// Performance Structure
struct Performance {
    int id;
    int rating;
    char remarks[200];
};
//training ka struct
struct Training {
    int id;
    char course[100];
    char date[20];
    char status[20];
};

// functions to avoid repetitive codes :p
int getNextEmployeeID() {
    FILE *fp = fopen(emp_f, "r");
    if (!fp) return 1;

    struct Employee temp;
    int lastID = 0;

    while (fscanf(fp, "%d %s %f %19s", &temp.id, temp.name, &temp.salary, temp.contact) == 4) {
        lastID = temp.id;
    }

    fclose(fp);
    return lastID + 1;
}

int idCheck(int tempId){
	
	struct Employee check;
	
	FILE *fileP = fopen(emp_f, "r");
	
    while (fscanf(fileP, "%d %s %f %19s", &check.id, check.name, &check.salary, check.contact) == 4)
    {
        if (check.id == tempId) {
        	fclose(fileP);
            return 0;
        }
    }
    fclose(fileP);
    printf("Id Not Found\n");
    return 1;
}

//LOGIN
int login(char username[50],char pass[30]){

    if(strcmp(username,"missIzzah") == 0 && strcmp(pass,"best") == 0){
        printf("Log-In successful!\n");
        return 0;
    }
    printf("Incorrect Log-In Credentials\n");
    return 1;
}

//ADD, DELETE,LIST EMPLOYEES
void addemployee() {
    struct Employee e;
    char option; 
    bool valid;

    FILE *fp = fopen(emp_f, "a");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    e.id = getNextEmployeeID();
    fflush(stdin);
    do{

    printf("Enter Employee Name: ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = '\0';
    printf("Employee Name: %s.\n (Y\\N):", e.name);
	scanf("%c", &option);
	getchar();
	option = tolower(option);
	}while(option != 'y');
	
	do{
    printf("Enter Salary: ");
    scanf("%f", &e.salary);
    getchar();
   	printf("Employee salary: %.2f .\n (Y\\N):", e.salary);
	scanf("%c", &option);
	getchar();
	option = tolower(option);
	}while(option != 'y');
	
    printf("Enter Contact Number: ");
    do{
    valid = true;
    fgets(e.contact, sizeof(e.contact), stdin);
    e.contact[strcspn(e.contact, "\n")] = '\0';
    int len = strlen(e.contact);
    if (len < 10 || len > 15) {
            printf("Invalid input. Contact number must be between 10 and 15 digits.\n");
            valid = false;
    }
    for (int i = 0; i < len; i++) {
            if (!isdigit(e.contact[i])) {
                printf("Invalid input. Contact number contain  digits.\n");
                valid = false;
        }
    }
   }while(valid == false);

    fprintf(fp, "%d %s %.2f %s\n", e.id, e.name, e.salary, e.contact);

    fclose(fp);
    printf("Employee Added!\n");
    printf("ID: %d", e.id);
}

void deleteEmployee(){
    struct Employee temp;
    int id;

    FILE *fp = fopen(emp_f, "a+");
    FILE *tempf = fopen("temp.txt", "w");
    rewind(fp);


    if (!fp) {
        printf("\nNo Employee Data\n");
        return;
    }

    printf("Enter Employee ID To Delete: ");
    scanf("%d", &id);

    bool found = false;
    int check = idCheck(id);
    if(!check){

    while (fscanf(fp, "%d %s %f %19s", &temp.id, temp.name, &temp.salary, temp.contact) == 4)
    {
        if (temp.id == id) {
            found = true;
            continue;
        }

        fprintf(tempf, "%d %s %.2f %s\n", temp.id, temp.name, temp.salary, temp.contact);
    }

    fclose(fp);
    fclose(tempf);

    remove(emp_f);
    rename("temp.txt", emp_f);

    printf("Employee deleted successfully.\n");
    }
}

void listEmployees() {
    FILE *fp = fopen(emp_f, "r");
    struct Employee e;

    if (!fp) {
        printf("No employee data.\n");
        return;
    }

    printf("\n=====>EMPLOYEES<=====\n");

    while (fscanf(fp, "%d %99s %f %19s", &e.id, e.name, &e.salary, e.contact) == 4) {
        printf("ID: %d\nName: %s\nSalary: %.2f\nContact: %s\n\n", e.id, e.name, e.salary, e.contact);
    }
    fclose(fp);
}

//ATTENDANCE management
void addAttendance() {
    int id;
    char status;
    char date[20];

    FILE *fp = fopen(attend_f, "a");

    if (!fp) {
        printf("Error opening attendance file!\n");
        return;
    }
    
    printf("Enter Employee ID: ");
    scanf("%d", &id);
    getchar();
	if (idCheck(id)) { printf("Employee ID not found!\n"); fclose(fp); return; }
    printf("Enter Date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    printf("Enter Status (P/A/L): ");
    scanf(" %c", &status);

    fprintf(fp, "%d %s %c\n", id, date, status);

    fclose(fp);
    printf("Attendance Recorded!\n");
}

void viewAttendance() {
    FILE *fp = fopen(attend_f, "r");
    int id;
    char date[20], status;

    if (!fp) {
        printf("No attendance records.\n");
        return;
    }

    printf("\n===== ATTENDANCE =====\n");

    while (fscanf(fp, "%d %s %c", &id, date, &status) == 3) {
        printf("ID: %d  Date: %s  Status: %c\n", id, date, status);
    }

    fclose(fp);
}

void attendancePercentage() {
    int id;
    int total = 0, present = 0;

    printf("Enter Employee ID: ");
    scanf("%d", &id);

    FILE *fp = fopen(attend_f, "r");

    if (!fp) {
        printf("No attendance data.\n");
        return;
    }

    int tempID;
    char date[20], status;

    while (fscanf(fp, "%d %s %c", &tempID, date, &status) == 3) {
        if (tempID == id) {
            total++;
            status = toupper(status);
            if (status == 'P')
                present++;
        }
    }

    fclose(fp);

    if (total == 0) {
        printf("No attendance records for this employee.\n");
        return;
    }

    float percent = (float)present / total * 100;
    printf("Attendance: %.2f%%\n", percent);
}

//leave
void leaveRequests() {
	int id, tempID;
	int leaveDays;
	int total = 0;
	
	printf("Enter Employee ID: ");
    scanf("%d", &id);

    FILE *fp = fopen(leave_f, "r");
     
    while (fscanf(fp, "%d %d", &tempID, &leaveDays) == 2) {
    	if(id == tempID){
    	total = total + leaveDays;
		}
	}
	
	if(total > 20) {
		printf("Further Leaves Not Allowed\n");
		fclose(fp);
		return;
	}
	
	fclose(fp);
	
    fp = fopen(leave_f, "a");
	
	if (!fp) {
        printf("Error opening leave file for writing\n");
        return;
    }
    
    printf("Enter Number Of Days Required For Leave Application: ");
	scanf("%d", &leaveDays);
	
    if(total + leaveDays > 20) {
		printf("Not Enough Leaves Left\nLeave Rejected\n");
		return;
	}
	
	fprintf(fp, "%d %d\n", id, leaveDays);
	fclose(fp);
	printf("Leave Approved :)\n");
} 

void viewLeaves(){
	int tempID, leaveDays;
	FILE *fp = fopen(leave_f, "r");
    if (!fp) {
        printf("No Leave data.\n");
        return;
    }
	while (fscanf(fp, "%d %d", &tempID, &leaveDays) == 2) {
    	printf("\nID: %d\nLeave Days: %d\n",tempID, leaveDays);
    }
    fclose(fp);
}

//BACKUP & RECOVER EMPLOYEES FILE
void backup() {
	const char *employees = emp_f;
    const char *backup = backup_f;

    FILE *fp = fopen(employees, "r");
    if (!fp) {
        printf("Error in opening file\n");
        return;
    }

    FILE *fp2 = fopen(backup, "w");
    if (!fp2) {
        printf("Error backing up\n");
        fclose(fp);
        return;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        fputc(ch, fp2);
    }

    fclose(fp);
    fclose(fp2);

    printf("Backup successful\n");
}

void recover() {
    const char *employees = emp_f;
    const char *backup = backup_f;

    FILE *fp = fopen(backup, "r");
    if (!fp) {
        printf("No Data backup\n");
        return;
    }

    FILE *fp2 = fopen(employees, "w");
    
    if (!fp2) {
        printf("Error recovering file\n");
        fclose(fp);
        return;
    }

    char ch;
    
    while ((ch = fgetc(fp)) != EOF) {
        fputc(ch, fp2);
    }

    fclose(fp);
    fclose(fp2);

    printf("Recovery successful\n");
}

// PERFORMANCE MANAGEMENT
void addPerformance() {
    struct Performance p;
    FILE *fp = fopen(perform_f, "a");
    if (!fp) { printf("Error opening performance file!\n"); return; }

    printf("Enter Employee ID: ");
    scanf("%d", &p.id);

    if (idCheck(p.id)) { printf("Employee ID not found!\n"); fclose(fp); return; }

    printf("Enter Performance Rating (1-10): ");
    scanf("%d", &p.rating);
    getchar();

    printf("Enter Remarks: ");
    fgets(p.remarks, sizeof(p.remarks), stdin);
    p.remarks[strcspn(p.remarks, "\n")] = '\0';

    fprintf(fp, "%d %d %s\n", p.id, p.rating, p.remarks);
    fclose(fp);
    printf("Performance Recorded!\n");
}

void performanceReport() {
    FILE *fp = fopen(perform_f, "r");
    if (!fp) { printf("No performance records.\n"); return; }

    struct Performance p;
    printf("\n====== PERFORMANCE REPORT ======\n");
    while (fscanf(fp, "%d %d %199[^\n]", &p.id, &p.rating, p.remarks) == 3) {
        printf("ID: %d | Rating: %d | Remarks: %s\n", p.id, p.rating, p.remarks);
    }
    fclose(fp);
}

//payroll management
int calcPayroll(){

    int id;
    float basic,deductions,net;
    printf("\nEnter Employee ID: ");
    scanf("%d",&id);

    FILE *fp = fopen(emp_f,"r");
    if(!fp){
        printf("File is Empty!");
        return 0;
    }
    bool found = false;

    struct Employee e;    
    while (fscanf(fp, "%d %s %f %19s", &e.id, e.name, &e.salary, e.contact) == 4) {
        if(e.id == id){
            found = true; 
            break;
        }
    }
    fclose(fp);

    if(!found){
        printf("\nNo employee ID detected!");
        return 0;
    }

    basic = e.salary;
    printf("\nEnter Amount of Deduction:");
    scanf("%f",&deductions);
    if(deductions < 0.0 || deductions > basic){
        printf("invalid amount of deductions!\n");
        return 0;
    }
    net = basic - deductions;
    FILE *pof = fopen(pay_f,"a");
    if(!pof){
        printf("Error Opening File!\n");
        return 0;
    }

    fprintf(pof, "%d %s %.2f %.2f %.2f\n", e.id, e.name, basic, deductions, net); // FIXED
    fclose(pof);
    printf("Payroll Calculated and Saved!\n"); // FIXED

    return 0;
}

void generatePayslip() {
    int id;
    printf("Enter Employee ID: ");
    scanf("%d", &id);

    FILE *pf = fopen(pay_f, "r");
    if (!pf) {
        printf("No payroll data available.\n");
        return;
    }

    int tempID;
    char name[100];
    float basic, deduction, net;
    bool found = false;

    while (fscanf(pf, "%d %99s %f %f %f", &tempID, name, &basic, &deduction, &net) == 5) {
        if (tempID == id) {
            found = true;
            printf("\n===== PAYSLIP =====\n");
            printf("Employee ID: %d\n", tempID);
            printf("Name: %s\n", name);
            printf("Basic Salary: %.2f\n", basic);
            printf("Deductions: %.2f\n", deduction);
            printf("Net Salary: %.2f\n", net);
            printf("===================\n");
            break;
        }
    }

    fclose(pf);

    if (!found) {
        printf("Payslip not found for this employee.\n");
    }
}

void payrollReport() {
    FILE *pf = fopen(pay_f, "r");
    if (!pf) {
        printf("No payroll records.\n");
        return;
    }

    printf("\n=======>PAYROLL REPORT<=======\n");
    int id;
    char name[100];
    float basic, deduction, net;

    while (fscanf(pf, "%d %99s %f %f %f", &id, name, &basic, &deduction, &net) == 5) {
        printf("ID: %d | Name: %s | Basic: %.2f | Deduction: %.2f | Net: %.2f\n",
               id, name, basic, deduction, net);
    }

    fclose(pf);
}

//Recruitment Management
void candidatesApplication(){
    struct Employee e;
    struct Employee temp;
    char tempStatus, status = 'p';
    char option;
    int count = 0;
    bool valid = true;
    FILE *fp = fopen(job_f, "a+");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    printf("\n===Candidate Application===\n");

    getchar(); // to fix leftover newline
    do{
    printf("Enter Employee Name: ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = '\0';
    printf("Employee Name: %s.\n (Y\\N):", e.name);
	scanf("%c", &option);
	getchar();
	option = tolower(option);
	}while(option != 'y');


    printf("Enter Contact Number: ");
    do{
    valid = true;
    fgets(e.contact, sizeof(e.contact), stdin);
    e.contact[strcspn(e.contact, "\n")] = '\0';
    int len = strlen(e.contact);
    if (len < 10 || len > 15) {
            printf("Invalid input. Contact number must be between 10 and 15 digits.\n");
            valid = false;
    }
    for (int i = 0; i < len; i++) {
            if (!isdigit(e.contact[i])) {
                printf("Invalid input. Contact number contain  digits.\n");
                valid = false;
        }
    }
   }while(valid == false); 
         
    rewind(fp);
    
    while (fscanf(fp, "%d %s %c %19s", &temp.id, temp.name, &tempStatus, temp.contact) == 4)
    {
        if (strcmp(temp.contact, e.contact) == 0) {
            printf("User already applied\n");
            fclose(fp);
            return;
        }
    	count = temp.id; 
	}

    e.id = count + 1;
    
    fseek(fp, 0, SEEK_END);

    fprintf(fp, "%d %s %c %s\n", e.id, e.name, status, e.contact);
    fclose(fp);
    printf("Application submitted. Your ID is: %d\n", e.id);

}

void interviewScheduling() {
    struct Employee temp;
    int id;  
	bool found = false;
    char tempStatus,newStatus = 'i';
    char date[20];

    FILE *fp = fopen(job_f, "r");
    if(!fp){
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID to be scheduled for interview: ");
    scanf("%d", &id);
    getchar();

    while (fscanf(fp, "%d %s %c %19s", &temp.id, temp.name, &tempStatus, temp.contact) == 4){
        if(id == temp.id){
            if(tempStatus == 'i'){
                printf("User already scheduled\n");
                fclose(fp);
                return;
            }
            found = true;
            break;
        }
    }
    rewind(fp);

    if (!found) {
        printf("ID Not Found\n");
        fclose(fp);
        return;
    }

    printf("Enter Date ForInterview (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    FILE *fp2 = fopen(app_f, "a");
    if (!fp2) {
        printf("Error opening interview file!\n");
        fclose(fp);
        return;
    }
    fprintf(fp2, "%d %s\n", id, date);
    fclose(fp2);

    FILE *tempf = fopen("temp.txt", "w");
    if (!tempf) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d %s %c %19s",&temp.id, temp.name, &tempStatus, temp.contact) == 4){
        if (temp.id == id) {
            tempStatus = newStatus;
        }
        fprintf(tempf, "%d %s %c %s\n", temp.id, temp.name, tempStatus, temp.contact);
    }
    fclose(fp);
    fclose(tempf);

    remove(job_f);
    rename("temp.txt", job_f);

    printf("Interview successfully scheduled and status updated!\n");
}

void viewApplications(){
	struct Employee temp;
	char status;
	
	FILE *fp = fopen(job_f, "r");
	printf("p: pending\ni: interview\nc: confirmed");
	while (fscanf(fp, "%d %s %c %19s",&temp.id, temp.name, &status, temp.contact) == 4){
        printf("ID: %d  Name: %s  Status: %c  Contact: %s\n", temp.id, temp.name, status, temp.contact);
	}
	
	fclose(fp);
}

void confirmApplicant(){
	struct Employee temp;
	struct Employee e;
	char tempStatus;
	int id;
	bool found = false;
	char option;
	
	printf("Enter ID to confirm applicants job: ");
	scanf("%d", &id);
	getchar();
	
	FILE *fp = fopen(job_f, "r");
	if(!fp){
        printf("Error opening file!\n");
        return;
    }
	
	FILE *tempf = fopen("temp.txt", "w");
    if (!tempf) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

	while (fscanf(fp, "%d %s %c %19s", &temp.id, temp.name, &tempStatus, temp.contact) == 4){
        if(id == temp.id){
        	found = true;
        	tempStatus = 'c';
        	e.id = getNextEmployeeID(); //FIXED. calling a pre declared func.
        	strcpy(e.name, temp.name);
            strcpy(e.contact, temp.contact);
        }
        fprintf(tempf, "%d %s %c %s\n", temp.id, temp.name, tempStatus, temp.contact);
    }
    fclose(fp);
    fclose(tempf);
    remove(job_f);
    rename("temp.txt", job_f);
    
    if (found == false){
    	printf("ID Not Found\n");
    	return;
	}
    
    fp = fopen(emp_f, "a");
    
	if(!fp){
        printf("Error opening file!\n");
        return;
    }
    
    do{
    printf("Enter Salary: ");
    scanf("%f", &e.salary);
    getchar();
   	printf("Employee salary: %.2f .\n (Y\\N):", e.salary);
	scanf("%c", &option);
	getchar();
	option = tolower(option);
	}while(option != 'y');
	fprintf(fp, "%d %s %.2f %s\n", e.id, e.name, e.salary, e.contact);  
	fclose(fp);
}

//Training Management
void addTraining() {
    struct Training t;
    int id;

    printf("Enter Employee ID: ");
    scanf("%d", &id);
    getchar();
    if (idCheck(id)) {
        printf("Employee ID not found!\n");
        return;
    }
    printf("Enter Training Course: ");
    
    fgets(t.course, sizeof(t.course), stdin);
    t.course[strcspn(t.course, "\n")] = '\0';

    printf("Enter Training Date (YYYY-MM-DD): ");
    fgets(t.date, sizeof(t.date), stdin);
    t.date[strcspn(t.date, "\n")] = '\0';

    strcpy(t.status, "Pending");

    FILE *fp = fopen(train_f, "a");
    if (!fp) {
        printf("Error opening training file!\n");
        return;
    }

    fprintf(fp, "%d | %s | %s | %s\n", id, t.course, t.date, t.status);
    fclose(fp);

    printf("Training assigned to employee!\n");
}

void viewTraining() {
    FILE *fp = fopen(train_f, "r");
    if (!fp) {
        printf("No training records found.\n");
        return;
    }
    int id;
    char course[100], date[20], status[20];

    printf("\n===== TRAINING RECORDS =====\n");
    while (fscanf(fp, "%d | %99[^|] | %19[^|] | %19[^\n]",&id, course, date, status) == 4) {
        printf("ID: %d\nCourse: %s\nDate: %s\nStatus: %s\n\n",id, course, date, status);
    }
    fclose(fp);
}

void updateTrainingStatus() {
    int id;
    char newStatus[20];
    char inputStatus;
    char course[100], date[20], status[20];
    int tempID;
    char tempDate[20]; 

    printf("Enter Employee ID: ");
    scanf("%d", &id);
    getchar();
    
    printf("Enter Date(YYYY-MM-DD): ");
    scanf("%s", tempDate);
    getchar();
    tempDate[strcspn(tempDate, "\n")] = '\0';

    FILE *fp = fopen(train_f, "r");
    if (!fp) {
        printf("No training data.\n");
        return;
    }

    FILE *tempf = fopen("temp.txt", "w");
    if (!tempf) {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    printf("Enter new status (c: Completed /o: Ongoing /p: Pending): ");
    scanf("%c", &inputStatus);
    getchar();
    inputStatus = tolower(inputStatus);
    if(inputStatus == 'c'){
    	strcpy(newStatus ,"Completed");
	}
	else if(inputStatus == 'o'){
    	strcpy(newStatus ,"Ongoing");
	}
	else if(inputStatus == 'p'){
    	strcpy(newStatus ,"Pending");
	}

    while (fscanf(fp, "%d | %99[^|] | %19[^|] | %19[^\n]",&tempID, course, date, status) == 4) {
        while (isspace((unsigned char)date[0])) {
            memmove(date, date + 1, strlen(date)); //this logic moves one postion ahead in date if there is whitespace
        }
        char *end = date + strlen(date) - 1;
        while (end > date && isspace((unsigned char)*end)) {
            *end = '\0'; //this logic is used to remove whitespaces in the end of date
            end--;
        }
        if (tempID == id && strcmp(tempDate, date) == 0) {
            strcpy(status, newStatus);
        }

        fprintf(tempf, "%d | %s | %s | %s\n", tempID, course, date, status);
    }

    fclose(fp);
    fclose(tempf);

    remove(train_f);
    rename("temp.txt", train_f);

    printf("Training status updated!\n");
}

void trainingReport() {
    FILE *fp = fopen(train_f, "r");
    if (!fp) {
        printf("No training records.\n");
        return;
    }

    int id;
    char course[100], date[20], status[20];
    
    printf("\n===== TRAINING REPORT =====\n");
    while (fscanf(fp, "%d | %99[^|] | %19[^|] | %19[^\n]",&id, course, date, status) == 4) {

        printf("ID: %d | Course: %s | Date: %s | Status: %s\n",id, course, date, status);
    }
    fclose(fp);
}

//Menus sabke aleda aleda banai hai takay clean lagay
//Employee Menu
void employee_menu() {
    int ch;
    do {
        printf("\n--->>Employee Menu<<---\n");
        printf("1. Add Employee\n");
        printf("2. List Employees\n");
        printf("3. Delete Employee\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:  addemployee();  break;
            case 2:  listEmployees();  break;
            case 3:  deleteEmployee();  break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (ch != 0);
}

//Attendance Menu
void attendance_menu() {
    int ch;
    do {
        printf("\n--->>Attendance Menu<<---\n");
        printf("1. Mark Attendance\n");
        printf("2. View Attendance\n");
        printf("3. Attendance Percentage\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addAttendance(); break;
            case 2:  viewAttendance();  break;
            case 3:  attendancePercentage(); break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (ch != 0);
}

//Leave Menu
void leave_menu() {
    int ch;
    do {
        printf("\n--->>Leave Menu<<---\n");
        printf("1. Apply Leave\n");
        printf("2. View Leaves\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:  leaveRequests();  break;
            case 2: viewLeaves(); break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (ch != 0);
}

//Payroll Menu
void PayMenu() {
    int choice;
    do {
        printf("\n=====>>>>PAYROLL MENU<<<<=====\n");
        printf("1. Calculate Payroll\n");
        printf("2. Generate Payslip\n");
        printf("3. Payroll Report\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: calcPayroll(); break; // FIXED
            case 2: generatePayslip(); break;
            case 3: payrollReport(); break;
            case 0: break;
            default: printf("Invalid Option!\n");
        }

    } while(choice != 0);
}

//recruitment menu
void recruitment_menu(){
    int ch;
        do {
        printf("\n--->>Recruitment Menu<<---\n");
        printf("1. Add Candidate Application\n");
        printf("2. Schedule Interview\n");
        printf("3. View Application\n");
        printf("4. Confirm Applicants Status\n");
        printf("0. Back\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1:  candidatesApplication();  break;
            case 2:  interviewScheduling();  break;
            case 3:  viewApplications();  break;
            case 4:  confirmApplicant();  break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (ch != 0);
}

//Performance Menu
void performance_menu() {
    int ch;
    do {
        printf("\n--->>Performance Menu<<---\n");
        printf("1. Add Performance Record\n");
        printf("2. View Performance Report\n");
        printf("0. Back\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1:  addPerformance();  break;
            case 2:  performanceReport();  break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (ch != 0);
}

//Training Menu
void training_menu() {
    int ch;
    do {
        printf("\n--->>Training Menu<<---\n");
        printf("1. Assign Training\n");
        printf("2. View Training Records\n");
        printf("3. Update Training Status\n");
        printf("4. Generate Training Report\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        getchar(); 

        switch(ch) {
            case 1: addTraining();  break;
            case 2:  viewTraining();  break;
            case 3: updateTrainingStatus(); break;
            case 4: trainingReport(); break;
            case 0: break;
            default: printf("Invalid Choice!\n");
        }
    } while (ch != 0);
}

//Backup & Recovery Menu 
void backup_recovery_menu() {
    int c2;
    do {
        printf("\n1. Backup\n2. Recovery\n0. Back\nChoice: ");
        scanf("%d", &c2);
        switch (c2) {
            case 1: backup();  break;
            case 2:  recover();  break;
            case 0: break;
            default: printf("Invalid Choice!\n");
        }
    } while (c2 != 0);
}

//MENU
void menu(){
     int choice;
    char username[50], pass[30];
    int loginCheck;
	do{
	
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", pass);
    
	}while(login(username, pass) != 0);

    do {
        printf("\n--->>HR MANAGEMENT SYSTEM<<---\n");
        printf("1. Employee Management\n");
        printf("2. Attendance Management\n");
        printf("3. Leave Management\n");
        printf("4. Payroll Management\n");
         printf("5. Recruitment Management\n");
        printf("6. Performance Management\n");
        printf("7. Training Management\n");
        printf("8. Backup & Recovery\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: employee_menu(); break;
            case 2: attendance_menu(); break;
            case 3: leave_menu(); break;
            case 4: PayMenu(); break;
            case 5: recruitment_menu(); break;
            case 6: performance_menu(); break;
            case 7:training_menu(); break;
            case 8: backup_recovery_menu(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid Choice!\n");
        }
    } while (choice != 0);
}

// MAIN FUNCTION
int main() {
    menu();
    return 0;
}
