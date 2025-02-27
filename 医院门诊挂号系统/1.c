#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 宏定义数据最大值
#define Max_patientCount 1000
#define Max_doctorCount 1000
#define Max_departmentCount 1000
#define Max_appointmentCount 1000
#define Max_userCount 1000

// 信息保存的文件名
#define FILENAME1 "patient.txt"     // 患者信息保存
#define FILENAME2 "department.txt"  // 科室信息保存
#define FILENAME3 "doctor.txt"      // 医生信息保存
#define FILENAME4 "appointment.txt" // 预约信息保存
#define FILENAME5 "users.txt"       // 用户信息保存

// 用户结构体
typedef struct {
    char username[100];   // 用户名
    char password[100];   // 密码
} User;

// 患者结构体
typedef struct {
    int id;                    // 患者ID
    char name[30];             // 患者姓名
    char phoneNumber[30];      // 患者电话
    char gender[10];            // 患者性别（如 "M", "F"）
    char birthDate[20];        // 出生日期(格式为YYYY-MM-DD)
    char illness[1000];          // 患者病症
} Patient;

// 科室结构体
typedef struct {
    int id;                         // 科室ID
    char name[20];                  // 科室名称
    char location[100];             // 科室位置
    int doctorCount;                // 医生数量
    int doctorIds[Max_doctorCount]; // 科室包含的医生ID列表
} Department;

// 医生结构体
typedef struct {
    int id;                     // 医生ID
    char name[30];              // 医生姓名
    int departmentId;           // 医生所属科室ID
    int patientIds[100];        // 医生治疗的患者ID列表
    int patientCount;           // 治疗患者的数量
    char dutyHours[50];         // 医生值班时间（如 "周一至周五 8:00-17:00"）
} Doctor;

// 预约结构体
typedef struct {
    int id;                     // 预约ID
    int patientId;              // 患者ID
    int doctorId;               // 预约医生ID
    char appointmentDate[11];   // 预约日期（格式为YYYY-MM-DD）
} Appointment;

// 各个结构体
Patient patients[Max_patientCount];
Doctor doctors[Max_doctorCount];
Department departments[Max_departmentCount];
Appointment appointments[Max_appointmentCount];
User users[Max_userCount];

//初始化数量
int patientCount=0;
int doctorCount=0;
int departmentCount=0;
int appointmentCount=0;
int userCount=0;

void loadUsers();              // 加载用户
void saveUsers();              // 保存用户
int registerUser();            // 注册用户
int loginUser();               // 登录用户
void patientManagement();      // 患者管理
void loadPatients();           // 加载患者
void savePatients();           // 保存患者
void departmentManagement();   // 科室管理
void loadDepartments();        // 加载科室
void saveDepartments();        // 保存科室
void doctorManagement();       // 医生管理
void loadDoctors();            // 加载医生
void saveDoctors();            // 保存医生
void appointmentManagement();  // 预约管理
void loadAppointments();       // 加载预约
void saveAppointments();       // 保存预约

// 用户管理
void loadUsers() {
    FILE *file = fopen(FILENAME5, "r");
    if (!file) {
        return; // 文件不存在，直接返回
    }
    while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) == 2) {
        userCount++;
    }
    fclose(file);
}

void saveUsers() {
    FILE *file = fopen(FILENAME5, "w");
    if (!file) {
        perror("无法打开文件进行写入");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

int registerUser() {
    if (userCount >= Max_userCount) {
        printf("用户数量已达最大限制，无法注册新用户。\n");
        return 0;
    }
    User newUser;
    printf("请输入用户名：");
    scanf("%s", newUser.username);
    printf("请输入密码：");
    scanf("%s", newUser.password);
    
    // 检查用户名是否已存在
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("用户名已存在，请选择其他用户名。\n");
            return 0;
        }
    }
    
    users[userCount++] = newUser; // 添加新用户
    saveUsers();  // 保存用户信息
    printf("注册成功！\n");
    return 1;
}

int loginUser() {
    char username[100], password[100];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("登录成功！\n");
            return 1;
        }
    }
    printf("用户名或密码错误。\n");
    return 0;
}

// 患者
void loadPatients(){
    FILE *file=fopen(FILENAME1,"r");
    if(!file){
        return; // 文件不存在，直接返回
    }
    while(fscanf(file,"%d %29s %29s %9s %19s %999s",&patients[patientCount].id,patients[patientCount].name,patients[patientCount].phoneNumber,patients[patientCount].gender,patients[patientCount].birthDate,patients[patientCount].illness)==6){
        patientCount++;
    }
    fclose(file);
}

void savePatients(){
    FILE *file=fopen(FILENAME1,"w");
    if(!file){
        perror("无法打开文件进行写入");
        return;
    }
    for(int i=0;i<patientCount;i++){
        fprintf(file,"%d %s %s %s %s %s\n",patients[i].id,patients[i].name,patients[i].phoneNumber,patients[i].gender,patients[i].birthDate,patients[i].illness);
    }
    fclose(file);
}

void patientManagement(){                       
    int choice;
    do{
        printf("\n患者管理\n");
        printf("---------------\n");
        printf("  1. 增加患者\n");
        printf("  2. 删除患者\n");
        printf("  3. 查找患者\n");
        printf("  4. 修改患者信息\n");
        printf("  5. 全部患者信息统计\n");
        printf("  6. 返回上级菜单\n");
        printf("---------------\n");
        printf("请输入您的选择：");
        scanf("%d",&choice);
        switch(choice){
            case 1:{
                if(patientCount<Max_patientCount){
                    Patient newpatient;
                    newpatient.id=patientCount+1;
                    printf("请输入患者姓名：");
                    scanf("%s", newpatient.name);
                    printf("请输入患者电话：");
                    scanf("%s", newpatient.phoneNumber);
                    printf("请输入患者性别(M/F):");
                    scanf("%s", newpatient.gender);
                    printf("请输入患者出生日期(YYYY-MM-DD):");
                    scanf("%s", newpatient.birthDate);
                    printf("请输入患者病症："); 
                    scanf("%s",newpatient.illness);
                    patients[patientCount++] = newpatient;
                    printf("患者添加成功,患者ID为%d。\n",newpatient.id);
                    savePatients();  // 添加后保存患者信息             
                }else{
                    printf("超出最大患者数量,无法添加。\n");
                }
                break;
            }
            case 2:{
                int iddelete;
                printf("请输入要删除的患者ID:");
                scanf("%d", &iddelete);
                int found=0;
                for(int i=0;i<patientCount;i++){
                    if(patients[i].id==iddelete){
                        found=1;
                        for(int j=i;j<patientCount-1;j++){
                            patients[j]=patients[j+1];
                        }
                        patientCount--;
                        printf("删除患者成功。\n");
                        savePatients();  // 删除后保存
                        break;
                    }
                }
                if(!found){
                    printf("未找到此患者。");
                }
                break;
            }
            case 3:{
                int idsearch;
                printf("请输入要查找的患者ID:");
                scanf("%d", &idsearch);
                int found=0;
                for(int i=0;i<patientCount;i++) {
                    if(patients[i].id==idsearch) {
                        found=1;
                        printf("ID:%d\n姓名:%s\n电话:%s\n性别:%s\n出生日期:%s\n病症:%s\n",patients[i].id, patients[i].name, patients[i].phoneNumber,patients[i].gender, patients[i].birthDate,patients[i].illness);
                        break;
                    }
                }
                if(!found){
                    printf("未找到ID为%d的患者。\n",idsearch);
                }
                break;
            }
            case 4:{
                int idmodify;
                printf("请输入要修改的患者ID:");
                scanf("%d", &idmodify);
                int found=0;
                for(int i=0;i<patientCount;i++) {
                    if(patients[i].id==idmodify) {
                        found=1;
                        printf("病患当前信息:\n");
                        printf("ID:%d\n姓名:%s\n电话:%s\n性别:%s\n出生日期:%s\n病症:%s\n",patients[i].id, patients[i].name, patients[i].phoneNumber,patients[i].gender, patients[i].birthDate,patients[i].illness);
                        printf("请输入新的姓名：");
                        scanf("%s", patients[i].name);
                        printf("请输入新的电话：");
                        scanf("%s", patients[i].phoneNumber);
                        printf("请输入新的性别(M/F):");
                        scanf("%s", patients[i].gender);
                        printf("请输入新的出生日期(YYYY-MM-DD):");
                        scanf("%s", patients[i].birthDate);
                        printf("请输入患者病症："); 
                        scanf("%s",patients[i].illness);
                        printf("患者信息修改成功！\n");
                        savePatients();  // 修改后保存
                        break;
                    }
                }
                if(!found){
                    printf("未找到ID为%d的患者。\n",idmodify);
                }
                break;
            }
            case 5:{
                // 排序患者信息(ID升序)
                for(int i=0;i<patientCount-1;i++){
                    for(int j=0;j<patientCount-1-i;j++){
                        if(patients[j].id>patients[j+1].id){
                            Patient temp=patients[j];
                            patients[j]=patients[j+1];
                            patients[j+1]=temp;
                        }
                    }
                }
                printf("患者信息统计:\n");
                for(int i=0;i<patientCount;i++) {
                    printf("ID:%d,姓名:%s,电话:%s,性别:%s,出生日期:%s,病症:%s\n",patients[i].id, patients[i].name, patients[i].phoneNumber,patients[i].gender, patients[i].birthDate,patients[i].illness);
                }
                printf("\n总患者数量: %d\n",patientCount);
                break;
            }
            case 6:
                break;
            default:
                printf("无效选择，请重新输入。");
        }
    }while(choice!=6);
}

// 科室
void loadDepartments() {
    FILE *file=fopen(FILENAME2, "r");
    if (!file) {
        perror("无法打开科室文件");
        return;
    }
    while(fscanf(file, "%d %19s %99s", &departments[departmentCount].id, departments[departmentCount].name, departments[departmentCount].location) == 3) {
        // 读取医生数量
        fscanf(file, "%d", &departments[departmentCount].doctorCount);
        // 读取医生ID
        for (int j = 0; j < departments[departmentCount].doctorCount; j++) {
            fscanf(file, "%d", &departments[departmentCount].doctorIds[j]);
        }
        departmentCount++;
    }
    fclose(file);
}

void saveDepartments() {
    FILE *file=fopen(FILENAME2, "w");
    if(!file){
        perror("无法打开文件进行写入");
        return;
    }
    for(int i=0;i<departmentCount;i++){
        fprintf(file, "%d %s %s\n", departments[i].id, departments[i].name, departments[i].location);
        // 写入医生数量
        fprintf(file, "%d\n", departments[i].doctorCount);
        // 写入医生ID
        for(int j=0;j<departments[i].doctorCount;j++){
            fprintf(file, "%d ", departments[i].doctorIds[j]);
        }
        fprintf(file, "\n"); // 换行
    }
    fclose(file);
}

void departmentManagement() {
    int choice;
    do {
        printf("\n科室管理\n");
        printf("-------------------\n");
        printf(" 1. 增加科室\n");
        printf(" 2. 删除科室\n");
        printf(" 3. 查找科室\n");
        printf(" 4. 修改科室信息\n");
        printf(" 5. 全部科室信息统计\n");
        printf(" 6. 返回上级菜单\n");
        printf("-------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                if (departmentCount < Max_departmentCount) {
                    Department newDepartment;
                    newDepartment.id = departmentCount + 1; // 设置科室ID
                    printf("请输入科室名称：");
                    scanf("%s", newDepartment.name);
                    printf("请输入科室位置：");
                    scanf("%s", newDepartment.location);
                    // 输入医生数量
                    printf("请输入医生数量：");
                    scanf("%d", &newDepartment.doctorCount);
                    if (newDepartment.doctorCount > Max_doctorCount) {
                        printf("医生数量超出限制，最大为%d。\n", Max_doctorCount);
                        break;
                    }
                    // 输入医生ID
                    for (int j = 0; j < newDepartment.doctorCount; j++) {
                        printf("请输入医生ID %d: ", j + 1);
                        scanf("%d", &newDepartment.doctorIds[j]);
                    }
                    departments[departmentCount++] = newDepartment;
                    printf("科室添加成功, 科室ID为%d。\n", newDepartment.id);
                    saveDepartments(); // 添加后保存科室信息
                } else {
                    printf("超出最大科室数量, 无法添加。\n");
                }
                break;
            }
            case 2: {
                int iddelete;
                printf("请输入要删除的科室ID:");
                scanf("%d", &iddelete);
                int found = 0;
                for (int i = 0; i < departmentCount; i++) {
                    if (departments[i].id == iddelete) {
                        found = 1;
                        for (int j = i; j < departmentCount - 1; j++) {
                            departments[j] = departments[j + 1];
                        }
                        departmentCount--;
                        printf("删除科室成功。\n");
                        saveDepartments(); // 删除后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到此科室。\n");
                }
                break;
            }
            case 3: {
                int idsearch;
                printf("请输入要查找的科室ID:");
                scanf("%d", &idsearch);
                int found = 0;
                for (int i = 0; i < departmentCount; i++) {
                    if (departments[i].id == idsearch) {
                        found = 1;
                        printf("ID:%d\n名称:%s\n位置:%s\n",departments[i].id,departments[i].name,departments[i].location);
                        printf("包含医生数量: %d\n", departments[i].doctorCount);
                        printf("包含医生ID:");
                        for (int j = 0; j < departments[i].doctorCount; j++) {
                            printf(" %d", departments[i].doctorIds[j]);
                        }
                        printf("\n");
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的科室。\n", idsearch);
                }
                break;
            }
            case 4: {
                int idmodify;
                printf("请输入要修改的科室ID:");
                scanf("%d", &idmodify);
                int found = 0;
                for (int i = 0; i < departmentCount; i++) {
                    if (departments[i].id == idmodify) {
                        found = 1;
                        printf("科室当前信息:\n");
                        printf("ID:%d\n名称:%s\n位置:%s\n",departments[i].id,departments[i].name,departments[i].location);
                        printf("包含医生数量: %d\n", departments[i].doctorCount);
                        printf("包含医生ID:");
                        for (int j = 0; j < departments[i].doctorCount; j++) {
                            printf(" %d", departments[i].doctorIds[j]);
                        }
                        printf("\n");
                        // 修改科室信息
                        printf("请输入新的名称：");
                        scanf("%s", departments[i].name);
                        printf("请输入新的位置：");
                        scanf("%s", departments[i].location);
                        // 更新医生数量和ID
                        printf("请输入新的医生数量：");
                        scanf("%d", &departments[i].doctorCount);
                        if (departments[i].doctorCount > Max_doctorCount) {
                            printf("医生数量超出限制，最大为%d。\n", Max_doctorCount);
                            break;
                        }
                        for (int j = 0; j < departments[i].doctorCount; j++) {
                            printf("请输入医生ID %d: ", j + 1);
                            scanf("%d", &departments[i].doctorIds[j]);
                        }
                        printf("科室信息修改成功！\n");
                        saveDepartments(); // 修改后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的科室。\n", idmodify);
                }
                break;
            }
            case 5: {
                printf("科室信息统计:\n");
                for (int i = 0; i < departmentCount; i++) {
                    printf("ID:%d, 名称:%s, 位置:%s\n",departments[i].id,departments[i].name,departments[i].location);
                    printf("包含医生数量: %d\n", departments[i].doctorCount);
                    printf("包含医生ID:");
                    for (int j = 0; j < departments[i].doctorCount; j++) {
                        printf(" %d", departments[i].doctorIds[j]);
                    }
                    printf("\n");
                }
                printf("\n总科室数量: %d\n", departmentCount);
                break;
            }
            case 6:
                break;
            default:
                printf("无效选择，请重新输入。\n");
        }
    } while (choice != 6);
}

// 医生
void loadDoctors() {
    FILE *file = fopen(FILENAME3, "r");
    if (!file) {
        perror("无法打开医生文件");
        return;
    }
    while (fscanf(file, "%d %29s %d %49s", &doctors[doctorCount].id, doctors[doctorCount].name, &doctors[doctorCount].departmentId, doctors[doctorCount].dutyHours) == 4) {
        // 读取患者数量
        fscanf(file, "%d", &doctors[doctorCount].patientCount);
        // 读取患者ID
        for (int j = 0; j < doctors[doctorCount].patientCount; j++) {
            fscanf(file, "%d", &doctors[doctorCount].patientIds[j]);
        }
        doctorCount++;
    }
    fclose(file);
}

// 保存医生信息
void saveDoctors() {
    FILE *file = fopen(FILENAME3, "w");
    if (!file) {
        perror("无法打开文件进行写入");
        return;
    }
    for (int i = 0; i < doctorCount; i++) {
        fprintf(file, "%d %s %d %s\n", doctors[i].id, doctors[i].name, doctors[i].departmentId, doctors[i].dutyHours);
        // 写入患者数量
        fprintf(file, "%d\n", doctors[i].patientCount);
        // 写入患者ID
        for (int j = 0; j < doctors[i].patientCount; j++) {
            fprintf(file, "%d ", doctors[i].patientIds[j]);
        }
        fprintf(file, "\n"); // 换行
    }
    fclose(file);
}

// 医生管理函数
void doctorManagement() {
    int choice;
    do {
        printf("\n医生管理\n");
        printf("-------------------\n");
        printf(" 1. 增加医生\n");
        printf(" 2. 删除医生\n");
        printf(" 3. 查找医生\n");
        printf(" 4. 修改医生信息\n");
        printf(" 5. 全部医生信息统计\n");
        printf(" 6. 返回上级菜单\n");
        printf("-------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);
        switch (choice) {
            case 1: { // 增加医生
                if (doctorCount < Max_doctorCount) {
                    Doctor newDoctor;
                    newDoctor.id = doctorCount + 1; // 设置医生ID
                    newDoctor.patientCount = 0; // 初始化患者数量
                    printf("请输入医生姓名:");
                    scanf("%9s", newDoctor.name);
                    printf("请输入医生所属科室ID:");
                    scanf("%d", &newDoctor.departmentId);
                    printf("请输入医生值班时间:");
                    scanf("%49s", newDoctor.dutyHours);
                    // 输入患者数量
                    printf("请输入患者数量：");
                    scanf("%d", &newDoctor.patientCount);
                    if (newDoctor.patientCount > Max_patientCount) {
                        printf("患者数量超出限制，最大为%d。\n", Max_patientCount);
                        break;
                    }
                    // 输入患者ID
                    for (int j = 0; j < newDoctor.patientCount; j++) {
                        printf("请输入患者ID %d: ", j + 1);
                        scanf("%d", &newDoctor.patientIds[j]);
                    }
                    doctors[doctorCount++] = newDoctor; // 添加新医生
                    printf("医生添加成功, 医生ID为%d。\n", newDoctor.id);
                    saveDoctors();  // 保存医生信息
                } else {
                    printf("超出最大医生数量, 无法添加。\n");
                }
                break;
            }
            case 2: { // 删除医生
                int iddelete;
                printf("请输入要删除的医生ID:");
                scanf("%d", &iddelete);
                int found = 0;
                for (int i = 0; i < doctorCount; i++) {
                    if (doctors[i].id == iddelete) {
                        found = 1;
                        for (int j = i; j < doctorCount - 1; j++) {
                            doctors[j] = doctors[j + 1]; // 移动后面的医生信息到前面
                        }
                        doctorCount--;
                        printf("删除医生成功。\n");
                        saveDoctors();  // 删除后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到此医生。\n");
                }
                break;
            }
            case 3: { // 查找医生
                int idsearch;
                printf("请输入要查找的医生ID:");
                scanf("%d", &idsearch);
                int found = 0;
                for (int i = 0; i < doctorCount; i++) {
                    if (doctors[i].id == idsearch) {
                        found = 1;
                        printf("ID:%d\n姓名:%s\n科室ID:%d\n值班时间:%s\n", doctors[i].id, doctors[i].name, doctors[i].departmentId, doctors[i].dutyHours);
                        printf("包含患者数量: %d\n", doctors[i].patientCount);
                        printf("包含患者ID:");
                        for (int j = 0; j < doctors[i].patientCount; j++) {
                            printf(" %d", doctors[i].patientIds[j]);
                        }
                        printf("\n");
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的医生。\n", idsearch);
                }
                break;
            }
            case 4: { // 修改医生信息
                int idmodify;
                printf("请输入要修改的医生ID:");
                scanf("%d", &idmodify);
                int found = 0;
                for (int i = 0; i < doctorCount; i++) {
                    if (doctors[i].id == idmodify) {
                        found = 1;
                        printf("医生当前信息:\n");
                        printf("ID:%d\n姓名:%s\n科室ID:%d\n值班时间:%s\n", doctors[i].id, doctors[i].name, doctors[i].departmentId, doctors[i].dutyHours);
                        printf("包含患者数量: %d\n", doctors[i].patientCount);
                        printf("包含患者ID:");
                        for (int j = 0; j < doctors[i].patientCount; j++) {
                            printf(" %d", doctors[i].patientIds[j]);
                        }
                        printf("\n");
                        // 修改医生信息
                        printf("请输入新的姓名：");
                        scanf("%9s", doctors[i].name);
                        printf("请输入新的科室ID：");
                        scanf("%d", &doctors[i].departmentId);
                        printf("请输入新的值班时间：");
                        scanf("%49s", doctors[i].dutyHours);
                        // 更新患者数量和ID
                        printf("请输入新的患者数量：");
                        scanf("%d", &doctors[i].patientCount);
                        if (doctors[i].patientCount > Max_patientCount) {
                            printf("患者数量超出限制，最大为%d。\n", Max_patientCount);
                            break;
                        }
                        for (int j = 0; j < doctors[i].patientCount; j++) {
                            printf("请输入患者ID %d: ", j + 1);
                            scanf("%d", &doctors[i].patientIds[j]);
                        }
                        printf("医生信息修改成功！\n");
                        saveDoctors(); // 修改后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的医生。\n", idmodify);
                }
                break;
            }
            case 5: { // 全部医生信息统计
                printf("医生信息统计:\n");
                for (int i = 0; i < doctorCount; i++) {
                    printf("ID:%d, 姓名:%s, 科室ID:%d, 值班时间:%s\n", doctors[i].id, doctors[i].name, doctors[i].departmentId, doctors[i].dutyHours);
                    printf("包含患者数量: %d\n", doctors[i].patientCount);
                    printf("包含患者ID:");
                    for (int j = 0; j < doctors[i].patientCount; j++) {
                        printf(" %d", doctors[i].patientIds[j]);
                    }
                    printf("\n");
                }
                printf("\n总医生数量: %d\n", doctorCount);
                break;
            }
            case 6:
                break;
            default:
                printf("无效选择，请重新输入。\n");
        }
    } while (choice != 6);
}

// 预约
void loadAppointments(){                            //预约时请先在患者管理内添加患者，获取患者id之后再来预约
    FILE *file=fopen(FILENAME4, "r");
    if(!file){
        return; // 文件不存在，直接返回
    }
    while(fscanf(file, "%d %d %d %10s",&appointments[appointmentCount].id,&appointments[appointmentCount].patientId, &appointments[appointmentCount].doctorId,appointments[appointmentCount].appointmentDate)==4) {
        appointmentCount++;
    }
    fclose(file);
}

void saveAppointments() {
    FILE *file=fopen(FILENAME4, "w");
    if(!file){
        perror("无法打开文件进行写入");
        return;
    }
    for(int i=0;i<appointmentCount;i++) {
        fprintf(file, "%d %d %d %s\n", appointments[i].id, appointments[i].patientId, appointments[i].doctorId, appointments[i].appointmentDate);
    }
    fclose(file);
}

void appointmentManagement(){
    int choice;
    do {
        printf("\n预约管理\n");
        printf("-------------------\n");
        printf("  1. 增加预约\n");
        printf("  2. 删除预约\n");
        printf("  3. 查找预约\n");
        printf("  4. 修改预约信息\n");
        printf("  5. 全部预约信息统计\n");
        printf("  6. 返回上级菜单\n");
        printf("-------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                if (appointmentCount < Max_appointmentCount) {
                    Appointment newAppointment;
                    printf("请输入患者ID:");
                    scanf("%d", &newAppointment.patientId);
                    printf("请输入医生ID:");
                    scanf("%d", &newAppointment.doctorId);
                    newAppointment.id=appointmentCount+1; // 设置预约ID
                    printf("请输入预约日期(YYYY-MM-DD):");
                    scanf("%s", newAppointment.appointmentDate);
                    appointments[appointmentCount++]=newAppointment;
                    printf("预约添加成功, 预约ID为%d。\n", newAppointment.id);
                    saveAppointments();  // 添加后保存预约信息
                } else {
                    printf("超出最大预约数量, 无法添加。\n");
                }
                break;
            }
            case 2: {
                int iddelete;
                printf("请输入要删除的预约ID:");
                scanf("%d", &iddelete);
                int found=0;
                for (int i=0;i<appointmentCount;i++) {
                    if (appointments[i].id==iddelete) {
                        found=1;
                        for(int j=i;j<appointmentCount-1;j++) {
                            appointments[j]=appointments[j+1];
                        }
                        appointmentCount--;
                        printf("删除预约成功。\n");
                        saveAppointments();  // 删除后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到此预约。\n");
                }
                break;
            }
            case 3: {
                int idsearch;
                printf("请输入要查找的预约ID:");
                scanf("%d", &idsearch);
                int found=0;
                for (int i=0;i<appointmentCount;i++) {
                    if (appointments[i].id==idsearch) {
                        found=1;
                        printf("预约ID:%d\n患者ID:%d\n医生ID:%d\n预约日期:%s\n",appointments[i].id,appointments[i].patientId,appointments[i].doctorId, appointments[i].appointmentDate);
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的预约。\n", idsearch);
                }
                break;
            }
            case 4: {
                int idmodify;
                printf("请输入要修改的预约ID:");
                scanf("%d", &idmodify);
                int found=0;
                for (int i=0;i<appointmentCount;i++) {
                    if (appointments[i].id==idmodify) {
                        found = 1;
                        printf("当前预约信息:\n");
                        printf("预约ID:%d\n患者ID:%d\n医生ID:%d\n预约日期:%s\n", appointments[i].id, appointments[i].patientId, appointments[i].doctorId, appointments[i].appointmentDate);
                        printf("请输入新的患者ID:");
                        scanf("%d", &appointments[i].patientId);
                        printf("请输入新的医生ID:");
                        scanf("%d", &appointments[i].doctorId);
                        printf("请输入新的预约日期(YYYY-MM-DD):");
                        scanf("%s", appointments[i].appointmentDate);
                        printf("预约信息修改成功！\n");
                        saveAppointments();  // 修改后保存
                        break;
                    }
                }
                if (!found) {
                    printf("未找到ID为%d的预约。\n", idmodify);
                }
                break;
            }
            case 5: {
                printf("预约信息统计:\n");
                for (int i = 0; i < appointmentCount; i++) {
                    printf("预约ID:%d, 患者ID:%d, 医生ID:%d, 预约日期:%s\n", appointments[i].id, appointments[i].patientId, appointments[i].doctorId, appointments[i].appointmentDate);
                }
                printf("\n总预约数量: %d\n", appointmentCount);
                break;
            }
            case 6:
                break;
            default:
                printf("无效选择，请重新输入。\n");
        }
    } while (choice != 6);
}


int main() {
    loadUsers();              // 加载用户信息
    loadPatients();           // 加载患者信息
    loadDepartments();        // 加载科室信息
    loadDoctors();            // 加载医生信息
    loadAppointments();       // 加载预约信息
    // 登录或注册
    int loggedIn = 0;
    while (!loggedIn) {
        printf("欢迎使用医院门诊预约系统\n");
        printf("1. 注册\n");
        printf("2. 登录\n");
        printf("请输入您的选择：");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loggedIn = loginUser();
        } else {
            printf("无效选择，请重新输入。\n");
        }
    }
    int choice;
    do{
        printf("\n医院门诊预约挂号系统\n");
        printf("---------------\n");
        printf("  1. 患者管理\n");
        printf("  2. 医生管理\n");
        printf("  3. 科室管理\n");
        printf("  4. 预约管理\n");
        printf("  5. 退出系统\n");
        printf("---------------\n");
        printf("请输入您的选择：");
        scanf("%d",&choice);
        switch (choice){
            case 1:
                patientManagement();       // 患者管理
                break;
            case 2:
                doctorManagement();        // 医生管理
                break;
            case 3:
                departmentManagement();    // 科室管理
                break;
            case 4:
                appointmentManagement();   // 预约管理
                break;
            case 5:
                printf("谢谢使用，再见！\n");
                break;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }while(choice!=5);
    return 0;
}