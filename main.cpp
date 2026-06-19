#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "mylib.h"
using namespace std;

	char filenameDSLTC[80]="D:\\PR_GR10\\DSLTC.txt";
	char filenameDSSV[200]="D:\\PR_GR10\\DSSV.txt";
	char filenameDSMH[200]="D:\\PR_GR10\\DSMH.txt";

struct MonHoc 
{
	char MAMH[11];
    char TENMH[51];
	int STCLT, STCTH;
};
struct nodeMH 
{
	MonHoc mh;
	nodeMH *left, *right;
};
typedef nodeMH* treeMH;


struct DangKy
{
	char MASV[16] ; 
    float DIEM; 
    bool huydangky = false;
};
struct nodeDK 
{
	DangKy dk;
	nodeDK *next;
};
typedef nodeDK* PTRDK;


struct LopTinChi 
{
    int MALOPTC;
    char  MAMH[11];
    char NienKhoa[10];  
    int Hocky, Nhom, sosvmin, sosvmax;
    bool huylop = false ;
    PTRDK dssvdk = NULL;	
};
const int MAX_LTC = 1000;
struct DSLTC 
{
	int n = 0;
	LopTinChi* nodes[MAX_LTC];
};
 

struct SinhVien 
{
	char MASV [16]; char HO[51] ; char TEN[11];
	char  PHAI[4]; char SODT[16]; char MALOP[16];
};
struct nodeSV 
{
	SinhVien sv;
	nodeSV *next;
};
typedef nodeSV* PTRSV;

const int so_item = 11;
const int dong = 5;
const int cot = 40 ;
char thucdon[so_item][100] = {
    "1. Quan ly lop tin chi",
    "2. Quan ly lop sinh vien",
    "3. Quan ly mon hoc",
    "4. Dang ky lop tin chi",
    "5. Huy lop tin chi",
    "6. Nhap / sua diem lop tin chi",
    "7. In DSSV dang ky lop tin chi",
    "8. In bang diem mon hoc cua 1 lop tin chi",
    "9. In diem trung binh khoa hoc cua 1 lop",
    "10. In bang diem tong ket cac mon cua 1 lop",
    "11. Luu du lieu & Ket thuc chuong trinh"
};

							

void AnConTro(bool an) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = !an; 
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void NhanDienConTro(bool status) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    GetConsoleCursorInfo(hOut, &lpCursor);
    lpCursor.bVisible = status;
    SetConsoleCursorInfo(hOut, &lpCursor);
}

void clearLine(int x, int y, int l) {
    gotoxy(x, y);
    for (int i = 0; i < l; i++) cout << " ";
    gotoxy(x, y);
}

void GetConsoleSize(int &width, int &height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void LayNienKhoaHocKyHienTai(char nienkhoa[], int &hocKy) 
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int thang = 1 + ltm->tm_mon;
    int nam = 1900 + ltm->tm_year;
    int namBatDau, namKetThuc;
    if (thang >= 1 && thang <= 6) 
	{
        hocKy = 2;
        namBatDau = nam - 1;
        namKetThuc = nam;
    }
    else 
	{
        hocKy = 1;
        namBatDau = nam;
        namKetThuc = nam + 1;
    }
    sprintf(nienkhoa, "%d-%d", namBatDau, namKetThuc);
}

const short chuoi_so_max = 290;

char* nhap_ma() {
    short dem = 0; 
    char *res = new char[chuoi_so_max + 1]; 
    char tmp;

    while(true) {
        tmp = getch(); 
        
        if (tmp == 27) { 
            res[0] = '\0'; 
            return res;   
        }

        if (((tmp >= '0' && tmp <= '9') || (tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z') 
            || tmp == '-' || tmp == '_') && dem < chuoi_so_max) 
        {
            tmp = toupper(tmp); 
            cout << tmp;        
            res[dem++] = tmp;   
        } 
        else if (tmp == 8 && dem > 0) { 
            cout << "\b \b";   
            dem--;             
            res[dem] = '\0';    
        }
        else if (tmp == 13 || tmp == 10) { 
            res[dem] = '\0';   
            break;             
        }
        else {
            cout << "\a";      
        }
    }
    return res; 
}

char* nhap_chu() {
    short dem = 0;
    char *res = new char[chuoi_so_max + 1];
    char tmp;

    while(true) {
        tmp = getch();             
        
        if (tmp == 27) { 
            res[0] = '\0'; 
            return res; 
        }

        if (tmp == ' ' && dem == 0) continue; 

        if (((tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z') || tmp == ' ') && dem < chuoi_so_max) 
        {   
            if (dem > 0 && res[dem - 1] == ' ' && tmp == ' ') continue;
            cout << tmp;
            res[dem++] = tmp;
        }
        else if (tmp == 8 && dem > 0) {  
            cout << "\b \b";          
            dem--;
            res[dem] = '\0';
        }
        else if ((tmp == 13 || tmp == 10) && dem > 0) { 
            res[dem] = '\0'; 
            break;
        }
        else {
            cout << "\a";                
        }
    }
    return res;
}

char* nhap_so() {
    short dem = 0;
    char *res = new char[chuoi_so_max + 1]; 
    char tmp;

    while(true) {
        tmp = getch();              
        
        if (tmp == 27) { 
            res[0] = '\0';
            return res;
        }
        
        if (tmp >= '0' && tmp <= '9' && dem < chuoi_so_max) {
            cout << tmp;
            res[dem++] = tmp;
        }
        else if (tmp == 8 && dem > 0) {  
            cout << "\b \b";         
            dem--;
            res[dem] = '\0';
        }
        else if (tmp == 13 || tmp == 10) {
            res[dem] = '\0';           
            break;
        }
        else {
            cout << "\a";            
        }
    }
    return res;
}
void CanGiua(string text, int dong_hien_tai) {
    int man_hinh_rong = 120; // �?ng b? d? r?ng m�n h�nh l?n nhu trong ?nh c?a b?n
    int cot_can_giua = (man_hinh_rong - text.length()) / 2;
    if (cot_can_giua < 0) cot_can_giua = 0;
    gotoxy(cot_can_giua, dong_hien_tai);
    cout << text;
}
int Menu(char td[so_item][100]) {
    int chon = 0;           
    int man_hinh_rong = 120; 
    
    int do_rong_long_khung = 65;
    string duong_vien = "+" + string(do_rong_long_khung + 4, '-') + "+";
    
    int cot_x = (man_hinh_rong - duong_vien.length()) / 2;
    if (cot_x < 0) cot_x = 0;

    string input_so = "1"; 

    system("cls");
    
    SetBGColor(0); SetColor(11); 
    gotoxy(cot_x, dong - 2); cout << "============================= MENU CHINH =============================";
    gotoxy(cot_x, dong - 1); cout << duong_vien;
    
    for (int i = 0; i < so_item; i++) {
        string noi_dung = td[i];
        while(!noi_dung.empty() && noi_dung.back() == ' ') noi_dung.pop_back();
        while (noi_dung.length() < do_rong_long_khung) noi_dung += " ";
        
        string inner = "  " + noi_dung + "  ";
        
        gotoxy(cot_x, dong + i);
        SetBGColor(0); SetColor(11); cout << "|"; // VIỀN XANH
        if (i == chon) {
            SetBGColor(4); SetColor(15); cout << inner; // NỀN ĐỎ
        } else {
            SetBGColor(0); SetColor(15); cout << inner; // NỀN ĐEN
        }
        SetBGColor(0); SetColor(11); cout << "|"; // VIỀN XANH
    }
    
    SetBGColor(0); SetColor(11); gotoxy(cot_x, dong + so_item); cout << duong_vien;
    SetColor(14); gotoxy(cot_x, dong + so_item + 1); 
    cout << "[UP/DOWN]: Di chuyen  |  [Phim so]: Chon nhanh  |  [ENTER]: Thuc hien"; 
    
    SetColor(15);
    gotoxy(cot_x, dong + so_item + 3);
    cout << "Nhap lua chon cua ban: [      ]"; 
    gotoxy(cot_x + 24, dong + so_item + 3); 
    SetColor(12); cout << input_so;

    while (true) {
        int k = _getch();
        int chon_cu = chon;
        bool co_di_chuyen = false;

        if (k == 0 || k == 224) {
            k = _getch();
            if (k == 72) { chon--; if (chon < 0) chon = so_item - 1; co_di_chuyen = true; input_so = to_string(chon + 1); } 
            else if (k == 80) { chon++; if (chon >= so_item) chon = 0; co_di_chuyen = true; input_so = to_string(chon + 1); }
        } 
        else if (k >= '0' && k <= '9') {
            input_so += (char)k;
            int val = atoi(input_so.c_str());
            if (val >= 1 && val <= so_item) { chon = val - 1; co_di_chuyen = true; }
            else {
                input_so = ""; input_so += (char)k;
                val = atoi(input_so.c_str());
                if (val >= 1 && val <= so_item) { chon = val - 1; co_di_chuyen = true; }
            }
        }
        else if (k == 8 && input_so.length() > 0) { 
            input_so.pop_back();
            if (input_so.length() > 0) chon = atoi(input_so.c_str()) - 1;
            else chon = 0;
            co_di_chuyen = true;
        }
        else if (k == 13) {
            SetBGColor(0); SetColor(7); 
            return chon + 1; 
        }

        if (co_di_chuyen && chon != chon_cu) {
            string nd_cu = td[chon_cu];
            while(!nd_cu.empty() && nd_cu.back() == ' ') nd_cu.pop_back();
            while (nd_cu.length() < do_rong_long_khung) nd_cu += " ";
            string inner_cu = "  " + nd_cu + "  ";
            
            gotoxy(cot_x, dong + chon_cu); 
            SetBGColor(0); SetColor(11); cout << "|";
            SetBGColor(0); SetColor(15); cout << inner_cu;
            SetBGColor(0); SetColor(11); cout << "|";

            string nd_moi = td[chon];
            while(!nd_moi.empty() && nd_moi.back() == ' ') nd_moi.pop_back();
            while (nd_moi.length() < do_rong_long_khung) nd_moi += " ";
            string inner_moi = "  " + nd_moi + "  ";
            
            gotoxy(cot_x, dong + chon); 
            SetBGColor(0); SetColor(11); cout << "|";
            SetBGColor(4); SetColor(15); cout << inner_moi;
            SetBGColor(0); SetColor(11); cout << "|";
        }
        
        SetBGColor(0); SetColor(15); gotoxy(cot_x + 24, dong + so_item + 3); cout << "   ";
        SetBGColor(0); SetColor(12); gotoxy(cot_x + 24, dong + so_item + 3); cout << input_so;
    }
}
int CountNode(treeMH t){
    if(t == NULL) return 0;
    return 1 + CountNode(t->left) + CountNode(t->right);
}

void TreeToArray(treeMH t, treeMH a[], int &i) 
{
    if (t == NULL) return;
    TreeToArray(t->left, a, i);
    a[i++] = t; 
    TreeToArray(t->right, a, i);
}

void SortTENMH(treeMH a[], int n) 
{
    for (int i = 0; i < n - 1; i++) 
	{
        int min = i;
        for (int j = i + 1; j < n; j++) 
		{
            if (strcmp(a[j]->mh.TENMH, a[min]->mh.TENMH) < 0) 
			{
                min = j;
            }
        }
        if (min != i) 
		{
            treeMH temp = a[i];
            a[i] = a[min];
            a[min] = temp;
        }
    }
}

treeMH TimMH(treeMH t, char mamh[])
{
    if(t == NULL) return NULL;    
    int kq = strcmp(mamh, t->mh.MAMH);
    if(kq == 0) 
        return t;       
    else if(kq < 0)
        return TimMH(t->left, mamh);   
    else return TimMH(t->right, mamh);  
}

void InDSMH_TangTheoTen(treeMH root) {
    if (root == NULL) {
        SetColor(14);
        cout << "Danh sach rong!\n";
        return;
    }
	int n = CountNode(root);
    treeMH *a = new treeMH[n];
    int index = 0;
    TreeToArray(root, a, index);
    SortTENMH(a, n);
	int soDongTrenTrang = 10; 
    int tongSoTrang = (n + soDongTrenTrang - 1) / soDongTrenTrang;
    int trangHienTai = 1;
    AnConTro(true); 
	system("cls");
	while (true) {
        int startX = 15;
        int startY = 2;
		SetColor(11);
        gotoxy(startX, startY);     cout << "========================== DANH SACH MON HOC ==========================";
        gotoxy(startX, startY + 1); cout << "|                        (SAP XEP TANG THEO TEN)                      |";
        gotoxy(startX, startY + 2); cout << "|                                                                     |";
        gotoxy(startX, startY + 3); cout << "=======================================================================";
        SetColor(11);
        gotoxy(startX, startY + 4);
        cout << "|" << setw(3)  << left << "STT"
     		<< " | " << setw(12) << left << "MA MON"
     		<< " | " << setw(30) << left << "TEN MON HOC"
     		<< " | " << setw(6)  << left << "STCLT"
     		<< " | " << setw(6)  << left << "STCTH" << "|";
        gotoxy(startX, startY + 5);
        cout << "-----------------------------------------------------------------------";
		int startNode = (trangHienTai - 1) * soDongTrenTrang;
        int endNode = min(startNode + soDongTrenTrang, n);
		for (int i = 0; i < soDongTrenTrang; i++) {
            int idx = startNode + i;
            gotoxy(startX, startY + 6 + i);
            if (idx < endNode) {
                SetColor(11); cout << "| ";
                SetColor(7);  cout << setw(3) << left << idx + 1;
				SetColor(11); cout << "| ";
                SetColor(7);  cout << setw(12) << left << a[idx]->mh.MAMH;
                SetColor(11); cout << " | ";
                SetColor(7);  cout << setw(30) << left << a[idx]->mh.TENMH;
                SetColor(11); cout << " | ";
                SetColor(7);  cout << setw(6) << left << a[idx]->mh.STCLT;
                SetColor(11); cout << " | ";
                SetColor(7);  cout << setw(6) << left << a[idx]->mh.STCTH;
                SetColor(11); cout << "|";
            } else {
                SetColor(11);
                cout << "|    |              |                                |        |       |";
            }
        }
        
        SetColor(11);
        gotoxy(startX, startY + 6 + soDongTrenTrang);
        cout << "-----------------------------------------------------------------------";
        SetColor(7);
        gotoxy(startX, startY + 7 + soDongTrenTrang);
        cout << "***********************************************************************";
        SetColor(14);
        gotoxy(startX, startY + 8 + soDongTrenTrang);
        cout << "Trang: " << trangHienTai << "/" << tongSoTrang << "         | [<-] Trang truoc | [->] Trang sau";
        gotoxy(startX, startY + 9 + soDongTrenTrang);
        cout << "Bam [ESC] de thoat.";
		char c = getch(); 
        if (c == 27) break; 
        if (c == -32) { 
            c = getch();
            if (c == 75 && trangHienTai > 1) trangHienTai--;          
            if (c == 77 && trangHienTai < tongSoTrang) trangHienTai++; 
        }
    }

    delete[] a;
    SetColor(7); 
}

int TimLTC(DSLTC ds, int maloptc ){
    for(int i=0; i<ds.n; i++){
        if (ds.nodes[i]->MALOPTC == maloptc)
        return i;
    }
    return -1;
}

PTRSV TimSV(PTRSV FirstSV, char masv[]){
	for(PTRSV p = FirstSV; p != NULL; p = p->next){
		if(strcmp(p->sv.MASV, masv) == 0)
			return p;
	}
	return NULL;
}

void BaoLoi (const char *s){
    int x=wherex() , y=wherey();
    gotoxy (10,24);
    cout <<s;
    Sleep(2000);
    gotoxy(10,24);
    clreol();
    gotoxy(x,y);
}

bool TonTaiLop(PTRSV FirstSV, char malop[]) {
    PTRSV p = FirstSV;
    while (p != NULL) {
        if (strcmp(p->sv.MALOP, malop) == 0)
            return true;
        p = p->next;
    }
    return false;
}
void Insert_Order_SV(PTRSV &FirstSV, SinhVien x)
{
    PTRSV p = new nodeSV;     
    p->sv = x;
    p->next = NULL;
    if (FirstSV == NULL || strcmp(x.MASV, FirstSV->sv.MASV) <0)
	{   
        p->next = FirstSV;
        FirstSV = p;
        return;
    }
    PTRSV q = FirstSV;
    while (q->next != NULL && strcmp(q->next->sv.MASV, x.MASV) <0)
    q = q->next;
    p->next = q->next;
    q->next = p;
}

bool NhapSV(PTRSV &FirstSV, SinhVien &sv) {
    char malop[20] = ""; 
    int screenW, screenH;
    GetConsoleSize(screenW, screenH);
    while (true) 
    {
        system("cls"); 
        int lW = 50, lH = 5;
        int lX = (screenW - lW) / 2;
        int lY = (screenH - lH) / 2;
        SetColor(11);
        gotoxy(lX, lY + 0); cout << "==================================================";
        gotoxy(lX, lY + 1); cout << "|                                                |";
        gotoxy(lX, lY + 2); cout << "|   NHAP MA LOP:                                 |";
        gotoxy(lX, lY + 3); cout << "|                                                |";
        gotoxy(lX, lY + 4); cout << "==================================================";
        SetColor(7); 
        gotoxy(lX, lY + 6); cout << "[ENTER]: Tiep tuc            [ESC]: Thoat ra Menu";
        SetColor(15);
        gotoxy(lX + 17, lY + 2); 
        char* resMa = nhap_ma(); 
        if (strlen(resMa) == 0) 
        {
            delete[] resMa;
            return false; 
        }
        strcpy(malop, resMa);
        delete[] resMa; 
        break; 
    }
    
    strcpy(sv.MASV, ""); strcpy(sv.HO, ""); strcpy(sv.TEN, "");
    strcpy(sv.PHAI, ""); strcpy(sv.SODT, ""); strcpy(sv.MALOP, malop);
    int column = 0; 
    char ch;
    int boardW = 60, boardH = 15;
    int startX = (screenW - boardW) / 2;
    int startY = (screenH - boardH) / 2;
    system("cls");
    
    while (true) 
    {
        SetColor(11); 
        gotoxy(startX, startY + 0);  cout << "================== NHAP THONG TIN SV ===================";
        gotoxy(startX, startY + 1);  cout << "|                                                      |";
        gotoxy(startX, startY + 2);  SetColor(11); cout << "| "; SetColor(14); cout << "Lop dang chon: " << left << setw(38) << malop; SetColor(11); gotoxy(startX + 55, startY + 2); cout << "|";
        gotoxy(startX, startY + 3);  cout << "|                                                      |"; 
        gotoxy(startX, startY + 4);  cout << "| 1. Ma sinh vien :                                    |";
        gotoxy(startX, startY + 5);  cout << "|                                                      |"; 
        gotoxy(startX, startY + 6);  cout << "| 2. Ho sinh vien :                                    |";
        gotoxy(startX, startY + 7);  cout << "|                                                      |"; 
        gotoxy(startX, startY + 8);  cout << "| 3. Ten sinh vien:                                    |";
        gotoxy(startX, startY + 9);  cout << "|                                                      |"; 
        gotoxy(startX, startY + 10); cout << "| 4. Phai (Nam/Nu):                                    |";
        gotoxy(startX, startY + 11); cout << "|                                                      |"; 
        gotoxy(startX, startY + 12); cout << "| 5. So dien thoai:                                    |";
        gotoxy(startX, startY + 13); cout << "|                                                      |"; 
        gotoxy(startX, startY + 14); cout << "========================================================";
        SetColor(7); 
        gotoxy(startX, startY + 16); cout << "[Up/Down]: Di chuyen  | [Enter]: Luu  |  [ESC]: Quay lai";
        
        SetColor(15);
        gotoxy(startX + 20, startY + 4);  cout << sv.MASV;
        gotoxy(startX + 20, startY + 6);  cout << sv.HO;
        gotoxy(startX + 20, startY + 8);  cout << sv.TEN;
        gotoxy(startX + 20, startY + 10); cout << sv.PHAI;
        gotoxy(startX + 20, startY + 12); cout << sv.SODT;
        
        switch (column) 
        {
            case 0: gotoxy(startX + 20 + (int)strlen(sv.MASV), startY + 4); break;
            case 1: gotoxy(startX + 20 + (int)strlen(sv.HO),   startY + 6); break;
            case 2: gotoxy(startX + 20 + (int)strlen(sv.TEN),  startY + 8); break;
            case 3: gotoxy(startX + 20 + (int)strlen(sv.PHAI), startY + 10); break;
            case 4: gotoxy(startX + 20 + (int)strlen(sv.SODT), startY + 12); break;
        }
        
        ch = getch();
        if (ch == -32 || ch == 0) 
        {
            ch = getch();
            if (ch == 72) { column = (column > 0) ? column - 1 : 4; continue; } 
            if (ch == 80) { column = (column < 4) ? column + 1 : 0; continue; } 
            continue;
        }
        
        if (ch == 27) break; 
        
        if (ch == 13) 
        {
            if (strlen(sv.MASV) == 0) { column = 0; continue; } 
            if (strlen(sv.HO) == 0)   { column = 1; continue; }
            if (strlen(sv.TEN) == 0)  { column = 2; continue; } 
            if (strlen(sv.PHAI) == 0) { column = 3; continue; }
            if (strlen(sv.SODT) == 0) { column = 4; continue; }
            
            Insert_Order_SV(FirstSV, sv);
            SetColor(10); gotoxy(startX + 10, startY + 18); 
            cout << "SUCCESS: Them sinh vien thanh cong!           ";
            Sleep(2000); 
            
            strcpy(sv.MASV, ""); strcpy(sv.HO, ""); strcpy(sv.TEN, "");
            strcpy(sv.PHAI, ""); strcpy(sv.SODT, "");
            column = 0; system("cls"); 
            continue; 
        }
        
        ungetch(ch);
        SetColor(15); 
        
        if (column == 0) 
		{
            clearLine(startX + 20, startY + 4, 30);
            char *s = nhap_ma();
            if (strlen(s) > 0) 
			{
                if (TimSV(FirstSV, s))
				{
                    SetColor(12); gotoxy(startX + 10, startY + 18); cout << "LOI: Ma sinh vien bi trung! Nhap lai.";
                    Sleep(2000); 
                    clearLine(startX + 10, startY + 18, 60);
                } else { strcpy(sv.MASV, s); column = 1; }
            }
            delete[] s;
        }
        else if (column == 1)
		{
            clearLine(startX + 20, startY + 6, 30);
            char *t = nhap_chu();
            if (strlen(t) > 0) { strcpy(sv.HO, t); column = 2; }
            delete[] t;
        }
        else if (column == 2) 
		{
            clearLine(startX + 20, startY + 8, 30);
            char *b = nhap_chu();
            if (strlen(b) > 0) { strcpy(sv.TEN, b); column = 3; }
            delete[] b;
        }
        else if (column == 3) 
		{
            clearLine(startX + 20, startY + 10, 30);
            char *n = nhap_chu();
            if (strlen(n) > 0) { strcpy(sv.PHAI, n); column = 4; }
            delete[] n;
        }
        else if (column == 4) 
		{        
            clearLine(startX + 20, startY + 12, 30);
            char *e = nhap_so(); 
            if (strlen(e) == 0) 
			{
                delete[] e;
            } 
            else 
			{
                bool hopLe = true;
                if (strlen(e) != 10) 
				{
                    hopLe = false;
                    SetColor(12);
                    gotoxy(startX + 10, startY + 18);
                    cout << "LOI: So dien thoai phai du 10 so!";
                    Sleep(2000); 
                    clearLine(startX + 10, startY + 18, 45);
                }
                
                if (hopLe) 
				{
                    PTRSV p = FirstSV;
                    while (p != NULL) 
					{
                        if (strcmp(p->sv.SODT, e) == 0) 
						{
                            hopLe = false;
                            SetColor(12); 
                            gotoxy(startX + 10, startY + 18);
                            cout << "LOI: So dien thoai da ton tai!";
                            Sleep(2000); 
                            clearLine(startX + 10, startY + 18, 45);
                            break;
                        }
                        p = p->next;
                    }
                }            
                if (hopLe) 
				{
                    strcpy(sv.SODT, e);
                }
                delete[] e;
            }
        }
    }
    return true;
}

int DemSoGoiY_MaSV(PTRSV FirstSV, char* input) {
    int count = 0;
    for (PTRSV p = FirstSV; p != NULL; p = p->next) {
        if (strlen(input) == 0 || strstr(p->sv.MASV, input) != NULL) {
            count++;
        }
    }
    return count;
}
    


void LayGoiY_MaSV(PTRSV FirstSV, char* input, PTRSV dsGoiY[], int &nGoiY) {
    nGoiY = 0;
    for (PTRSV p = FirstSV; p != NULL; p = p->next) {
        if (strlen(input) == 0 || strstr(p->sv.MASV, input) != NULL) {
            dsGoiY[nGoiY++] = p; 
        }
    }
}

void VeKhungGoiY(PTRSV dsGoiY[], int nGoiY, int pointer, int trang, int lX, int lY)
{
    int soDongTrang = 8;
    int batDau = trang * soDongTrang;
    int ketThuc = (batDau + soDongTrang > nGoiY) ? nGoiY : batDau + soDongTrang;
    int tongSoTrang = (nGoiY == 0) ? 1 : (nGoiY + soDongTrang - 1) / soDongTrang;

    SetColor(11);

    gotoxy(lX, lY + 3);
    cout << "+------+----------------+---------------------------------+-----------+";

    gotoxy(lX, lY + 4);
    cout << "| STT  |   MA SO SV     |           HO VA TEN             |   LOP     |";

    gotoxy(lX, lY + 5);
    cout << "+------+----------------+---------------------------------+-----------+";

    int currentY = lY + 6;

    for(int i = 0; i < soDongTrang; i++)
    {
        int idx = batDau + i;

        gotoxy(lX, currentY + i);

        if(idx < nGoiY)
        {
            string hoTen = string(dsGoiY[idx]->sv.HO) + " " + dsGoiY[idx]->sv.TEN;

            if(idx == pointer)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);

                cout << "| "
                     << left << setw(4) << idx + 1
                     << " | "
                     << left << setw(14) << dsGoiY[idx]->sv.MASV
                     << " | "
                     << left << setw(31)
                     << (hoTen.length() > 31 ? hoTen.substr(0,31) : hoTen)
                     << " | "
                     << left << setw(8)
                     << dsGoiY[idx]->sv.MALOP
                     << " |";

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else
            {
                SetColor(11); cout << "| ";
                SetColor(15); cout << left << setw(4) << idx + 1;
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(14) << dsGoiY[idx]->sv.MASV;
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(31) << (hoTen.length() > 31 ? hoTen.substr(0,31) : hoTen);
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(8) << dsGoiY[idx]->sv.MALOP;
                SetColor(11); cout << " |";
            }
        }
        else
        {
            SetColor(11); cout << "|";
            SetColor(7);  cout << "      ";
            SetColor(11); cout << "|";
            SetColor(7);  cout << "                ";
            SetColor(11); cout << "|";
            SetColor(7);  cout << "                                 ";
            SetColor(11); cout << "|";
            SetColor(7);  cout << "           ";
            SetColor(11); cout << "|";
        }
    }

    gotoxy(lX, currentY + soDongTrang);
    SetColor(11);
    cout << "+------+----------------+---------------------------------+-----------+";

    SetColor(14);
    gotoxy(lX + 32, currentY + soDongTrang + 1);
    cout << "Trang " << trang + 1 << "/" << tongSoTrang;
}

bool HieuChinhSV(PTRSV &FirstSV) 
{	char input[20] = ""; 
	int screenW, screenH; 
    GetConsoleSize(screenW, screenH);
    int lX = (screenW - 65) / 2, lY = (screenH - 15) / 2;
  	int nIn = 0; 
    int ptrSV = 0; 
	while (true) 
    {	
       	int nGoiY = DemSoGoiY_MaSV(FirstSV, input);
		int sSize = (nGoiY == 0) ? 1 : nGoiY;
		PTRSV* dsGoiY = new PTRSV[sSize];
		int nGoiYActual = 0;
		LayGoiY_MaSV(FirstSV, input, dsGoiY, nGoiYActual);
		
        system("cls");
        AnConTro(true);
        SetColor(11);
        gotoxy(lX, lY + 0); cout << "=======================================================================";
        gotoxy(lX, lY + 1); cout << "|                                                                     |";
        gotoxy(lX, lY + 2); cout << "|   NHAP MA SV CAN SUA:                                               |";
        gotoxy(lX, lY + 3); cout << "|                                                                     |";
        gotoxy(lX, lY + 4); cout << "=======================================================================";
        SetColor(7);
        gotoxy(lX, lY + 18); cout << "[Len/Xuong]: Chon                                          [ESC]: Thoat";
        gotoxy(lX, lY + 19); cout << "[Enter]: Sua SV                                 [Trai/Phai]: Sang trang";

        while (true) 
        {
            AnConTro(true);
            int trang = (nGoiY == 0) ? 0 : ptrSV / 8;
            int tongSoTrang = (nGoiY + 7) / 8;

            SetColor(15);
            gotoxy(lX + 24, lY + 2); cout << setw(15) << left << input;
            VeKhungGoiY(dsGoiY, nGoiY, ptrSV, trang, lX, lY + 2);
            AnConTro(false); 
            gotoxy(lX + 24 + nIn, lY + 2);
            
            char c = getch();
            if (c == 27) 
            { 
                SetColor(15);
                delete[] dsGoiY;
                return false;
            }
            if (c == 0 || (unsigned char)c == 224) 
            {
                c = getch();
                if (c == 72 && ptrSV > 0) ptrSV--; 
                else if (c == 80 && ptrSV < nGoiY - 1) ptrSV++; 
                else if (c == 75 && trang > 0) 
                {
                    ptrSV -= 8;
                    if (ptrSV < 0) ptrSV = 0;
                }
                else if (c == 77 && trang < tongSoTrang - 1) 
                {
                    ptrSV += 8;
                    if (ptrSV >= nGoiY) ptrSV = nGoiY - 1;
                }
                continue; 
            }
            else if (c == 13 && nGoiY > 0) break;
            else if (c == 8 && nIn > 0) 
            { 
                input[--nIn] = '\0'; 
                ptrSV = 0; 
                LayGoiY_MaSV(FirstSV, input, dsGoiY, nGoiY);
            }
            else if (isalnum(c) && nIn < 15) 
            { 
                input[nIn++] = toupper(c); 
                input[nIn] = '\0'; 
                ptrSV = 0; 
                LayGoiY_MaSV(FirstSV, input, dsGoiY, nGoiY);
            }
        }

        PTRSV p = TimSV(FirstSV, dsGoiY[ptrSV]->sv.MASV);
        if (p == NULL) continue;
        PTRSV svSua = p; 
        int col = 0; 
        bool backToSearch = false;
        system("cls");
        int bX = (screenW - 60) / 2, bY = (screenH - 20) / 2;
        SetColor(11);
        gotoxy(bX, bY + 0);  cout << "================== HIEU CHINH SINH VIEN ==================";
        gotoxy(bX, bY + 1);  cout << "|                                                        |";
        gotoxy(bX, bY + 2);  cout << "|"; SetColor(14); cout << " DANG SUA MA SO: " << svSua->sv.MASV; 
        SetColor(11); gotoxy(bX + 57, bY + 2); cout << "|";
        gotoxy(bX, bY + 3);  cout << "|                                                        |";
        gotoxy(bX, bY + 4);  cout << "| 0. Ho moi        :                                     |";
        gotoxy(bX, bY + 5);  cout << "|                                                        |";
        gotoxy(bX, bY + 6);  cout << "| 1. Ten moi       :                                     |";
        gotoxy(bX, bY + 7);  cout << "|                                                        |";
        gotoxy(bX, bY + 8);  cout << "| 2. Phai (Nam/Nu) :                                     |";
        gotoxy(bX, bY + 9);  cout << "|                                                        |";
        gotoxy(bX, bY + 10); cout << "| 3. So dien thoai :                                     |";
        gotoxy(bX, bY + 11); cout << "|                                                        |";
        gotoxy(bX, bY + 12); cout << "| 4. Lop           :                                     |";
        gotoxy(bX, bY + 13); cout << "==========================================================";
        SetColor(7);
        gotoxy(bX, bY + 15); cout << "[Up/Down]: Di chuyen      [Enter]: Luu       [ESC]: Thoat ";
        
        while (true) 
        {
            AnConTro(true);
            for (int i = 0; i < 5; i++) 
            {
                int dongIn = (i == 0) ? 4 : (i * 2 + 4);
                gotoxy(bX + 21, bY + dongIn);
                if (col == i) SetColor(143); else SetColor(15);
                switch (i) 
                {
                    case 0: cout << " " << setw(30) << left << svSua->sv.HO; break;
                    case 1: cout << " " << setw(30) << left << svSua->sv.TEN; break;
                    case 2: cout << " " << setw(30) << left << svSua->sv.PHAI; break;
                    case 3: cout << " " << setw(30) << left << svSua->sv.SODT; break;
                    case 4: cout << " " << setw(30) << left << svSua->sv.MALOP; break;
                }
            }
            AnConTro(false); SetColor(15);
            int cur = (col == 0) ? strlen(svSua->sv.HO) :
                      (col == 1) ? strlen(svSua->sv.TEN) :
                      (col == 2) ? strlen(svSua->sv.PHAI) :
                      (col == 3) ? strlen(svSua->sv.SODT) : strlen(svSua->sv.MALOP);
            int dongConTro = (col == 0) ? 4 : (col * 2 + 4);
            gotoxy(bX + 22 + cur, bY + dongConTro);
            
            int ch = getch();
            if (ch == 0 || (unsigned char)ch == 224) 
            {
                ch = getch();
                if (ch == 72) col = (col > 0) ? col - 1 : 4; 
                if (ch == 80) col = (col < 4) ? col + 1 : 0; 
                continue; 
            }
            if (ch == 27) 
            {   
                backToSearch = true; 
                SetColor(15);
                break;
            } 
            if (ch == 13) 
            {
                if (strlen(svSua->sv.HO) == 0) { col = 0; continue; }
                if (strlen(svSua->sv.TEN) == 0) { col = 1; continue; }
                if (strlen(svSua->sv.PHAI) == 0) { col = 2; continue; }
                if (strlen(svSua->sv.SODT) == 0) { col = 3; continue; }
                if (strlen(svSua->sv.MALOP) == 0) { col = 4; continue; }

                p = svSua; 
                SetColor(10); gotoxy(bX + 15, bY + 17);
                cout << "SUCCESS: DA LUU THANH CONG!           ";
                Sleep(2000); backToSearch = true; break; 
            }

            ungetch(ch);
            SetColor(15);
            
            if (col == 0) 
            {
                gotoxy(bX + 22, bY + 4); cout << "                              ";
                gotoxy(bX + 22, bY + 4);
                char *s = nhap_chu();
                if (strlen(s) > 0) {
                    strcpy(svSua->sv.HO, s);
                    col = 1;
                }
                delete[] s; fflush(stdin);
            }
            else if (col == 1) 
            {
                gotoxy(bX + 22, bY + 6); cout << "                              ";
                gotoxy(bX + 22, bY + 6);
                char *t = nhap_chu();
                if (strlen(t) > 0) {
                    strcpy(svSua->sv.TEN, t);
                    col = 2;
                }
                delete[] t; fflush(stdin);
            }
            else if (col == 2) 
            {
                gotoxy(bX + 22, bY + 8); cout << "                              ";
                gotoxy(bX + 22, bY + 8);
                char *sex = nhap_chu();
                if (strlen(sex) > 0) {
                    strcpy(svSua->sv.PHAI, sex);
                    col = 3;
                }
                delete[] sex; fflush(stdin);
            }
            else if (col == 3) 
            {
                gotoxy(bX + 22, bY + 10); cout << "                              ";
                gotoxy(bX + 22, bY + 10);
                char *tel = nhap_so();

                if (strlen(tel) > 0) {
                    bool hopLe = true;
                    if (strlen(tel) != 10) 
                    {
                        hopLe = false;
                        SetColor(12); gotoxy(bX + 10, bY + 17);
                        cout << "LOI: So dien thoai phai du 10 so!";
                        Sleep(2000); clearLine(bX + 10, bY + 17, 50);
                    }
                    if (hopLe) 
                    {
                        PTRSV tmp = FirstSV;
                        while (tmp != NULL) 
                        {
                            if (strcmp(tmp->sv.MASV, svSua->sv.MASV) != 0 && strcmp(tmp->sv.SODT, tel) == 0) {
                                hopLe = false;
                                SetColor(12); gotoxy(bX + 10, bY + 17);
                                cout << "LOI: So dien thoai da ton tai!";
                                Sleep(2000); clearLine(bX + 10, bY + 17, 50);
                                break;
                            }
                            tmp = tmp->next;
                        }
                    }
                    if (hopLe) { 
                        strcpy(svSua->sv.SODT, tel); 
                        col = 4;
                    }
                }
                delete[] tel; fflush(stdin);
            }
            else if (col == 4) 
            {
                gotoxy(bX + 22, bY + 12); cout << "                              ";
                gotoxy(bX + 22, bY + 12);
                char *a = nhap_ma();
                if (strlen(a) > 0) {
                    strcpy(svSua->sv.MALOP, a);
                }
                delete[] a; fflush(stdin);
            }
            if (backToSearch) break;
        }
        if (backToSearch) { input[0] = '\0'; nIn = 0; ptrSV = 0; col = 0; continue; }
        break;
        delete[] dsGoiY;
    }
    SetColor(7);
    return true;
}

void xoaSinhVien(PTRSV &First, char maXoa[]) 
{
    if (First == NULL) return;
	PTRSV p = First, prev = NULL;
    while (p != NULL && strcmp(p->sv.MASV, maXoa) != 0)
	{
        prev = p;
        p = p->next;
    }
    if (p != NULL) 
	{
        if (p == First) First = p->next; 
        else prev->next = p->next;      
        delete p;
    }
} 

bool XoaSV(PTRSV &FirstSV, DSLTC dsLTC) 
{
    
    char masv_chon[20] = "";
    int nGoiY = DemSoGoiY_MaSV(FirstSV, masv_chon);
	int sSize = (nGoiY == 0) ? 1 : nGoiY;
    while (true) 
    {
        int screenW, screenH; 
        GetConsoleSize(screenW, screenH);
        system("cls");
        int lX = (screenW - 65) / 2, lY = (screenH - 15) / 2;
        char input[20] = ""; 
        int nIn = 0; 
        int ptrSV = 0; 
        int trang = 0;
        PTRSV* dsGoiY = new PTRSV[sSize];
        
        while (true) 
        {	int nGoiYActual=0;
            LayGoiY_MaSV(FirstSV, input, dsGoiY, nGoiYActual);
            trang = ptrSV / 8; 
            AnConTro(true);
            SetColor(11);
            gotoxy(lX, lY + 0); cout << "=======================================================================";
            gotoxy(lX, lY + 1); cout << "|                                                                     |";
            gotoxy(lX, lY + 2); cout << "|   NHAP MA SV CAN XOA:                                               |";
            gotoxy(lX, lY + 3); cout << "|                                                                     |";
            gotoxy(lX, lY + 4); cout << "=======================================================================";
            
            VeKhungGoiY(dsGoiY, nGoiY, ptrSV, trang, lX, lY + 2); 
            
            SetColor(7);
            gotoxy(lX, lY + 18); cout << "[Len/Xuong]: Chon                             [Trai/Phai]: Sang trang";
            gotoxy(lX, lY + 19); cout << "[Enter]: Chon SV                              [ESC]: Thoat";
            SetColor(15);
            gotoxy(lX + 24, lY + 2); cout << input << "                ";
            AnConTro(false);
            gotoxy(lX + 24 + nIn, lY + 2); 
            
            int c = getch();
            if (c == 27) 
            {
                delete[] dsGoiY;
                SetColor(15);
                return false;
            }
            if (c == 0 || (unsigned char)c == 224) 
            {
                c = getch();
                if (c == 72 && ptrSV > 0) ptrSV--;                  
                else if (c == 80 && ptrSV < nGoiY - 1) ptrSV++; 
                else if (c == 77) { ptrSV += 8; if (ptrSV >= nGoiY) ptrSV = nGoiY - 1; }
                else if (c == 75) { ptrSV -= 8; if (ptrSV < 0) ptrSV = 0; }
            }
            else if (c == 13) 
            {
                if (nGoiY > 0) { strcpy(masv_chon, dsGoiY[ptrSV]->sv.MASV); break; }
                else if (strlen(input) > 0) { strcpy(masv_chon, input); break; }
            }
            else if (c == 8 && nIn > 0) { input[--nIn] = '\0'; ptrSV = 0; }
            else if (isalnum(c) && nIn < 15) { input[nIn++] = toupper(c); input[nIn] = '\0'; ptrSV = 0; }
        }

        if (strlen(masv_chon) > 0) 
        {
            PTRSV q = TimSV(FirstSV, masv_chon);
            if (q != NULL) 
            {
                system("cls");
                int bX = (screenW - 60) / 2, bY = (screenH - 20) / 2;
                SetColor(11);
                gotoxy(bX, bY + 0);  cout << "===================== XAC NHAN XOA SV ====================";
                gotoxy(bX, bY + 1);  cout << "|                                                        |";
                gotoxy(bX, bY + 2);  cout << "|"; SetColor(14); cout << " DANG CHON MA SO: " << q->sv.MASV; 
                SetColor(11); gotoxy(bX + 57, bY + 2); cout << "|";
                gotoxy(bX, bY + 3);  cout << "|                                                        |";
                gotoxy(bX, bY + 4);  cout << "| 1. Ho sinh vien  : "; SetColor(15); cout << q->sv.HO;
                SetColor(11); gotoxy(bX + 57, bY + 4); cout << "|";
                gotoxy(bX, bY + 5);  cout << "|                                                        |";
                gotoxy(bX, bY + 6);  cout << "| 2. Ten sinh vien : "; SetColor(15); cout << q->sv.TEN;
                SetColor(11); gotoxy(bX + 57, bY + 6); cout << "|";
                gotoxy(bX, bY + 7);  cout << "|                                                        |";
                gotoxy(bX, bY + 8);  cout << "| 3. Phai (Nam/Nu) : "; SetColor(15); cout << q->sv.PHAI;
                SetColor(11); gotoxy(bX + 57, bY + 8); cout << "|";
                gotoxy(bX, bY + 9);  cout << "|                                                        |";
                gotoxy(bX, bY + 10); cout << "| 4. So dien thoai : "; SetColor(15); cout << q->sv.SODT;
                SetColor(11); gotoxy(bX + 57, bY + 10); cout << "|";
                gotoxy(bX, bY + 11); cout << "|                                                        |";
                gotoxy(bX, bY + 12); cout << "==========================================================";

                bool cothexoa = true;
                for (int i = 0; i < dsLTC.n && cothexoa == true; i++) 
                {
                    PTRDK pDK = dsLTC.nodes[i]->dssvdk;
                    while (pDK != NULL) {
                        if (strcmp(pDK->dk.MASV, masv_chon) == 0 && pDK->dk.huydangky == false) 
                        {
                            SetColor(12); gotoxy(bX + 9, bY + 14);
                            cout << "LOI: SV DA DANG KY LTC . KHONG THE XOA!";
                            cothexoa = false;
                            Sleep(2000);
                            break;
                        }
                        pDK = pDK->next;
                    }
                }
                
                if (cothexoa)
                { 
                    int boxX = bX, boxY = bY + 14; 
                    SetColor(13); 
                    gotoxy(boxX, boxY + 0); cout << "__________________________________________________________";
                    gotoxy(boxX, boxY + 1); cout << "|                                                        |";
                    gotoxy(boxX, boxY + 2); 
                    string hoTen = string(q->sv.HO) + " " + q->sv.TEN;
                    cout << "|  XOA SV: " << left << setw(46) << hoTen << "|";
                    gotoxy(boxX, boxY + 3); cout << "|  Ban co chac chan muon xoa khong? (Y/N): _             |"; 
                    gotoxy(boxX, boxY + 4); cout << "|________________________________________________________|";
                    
                    gotoxy(boxX + 43, boxY + 3); 
                    char confirm = getch();
                    cout << (char)toupper(confirm); 
                    
                    if (confirm == 'y' || confirm == 'Y') 
                    {
                        xoaSinhVien(FirstSV, masv_chon);
                        SetColor(10); 
                        gotoxy(boxX + 6, boxY + 6); cout << "SUCCESS: DA XOA SINH VIEN KHOI HE THONG!";
                        Sleep(2000);
                    } 
                    else 
                    {
                        SetColor(14); 
                        gotoxy(boxX + 6, boxY + 6); cout << "DA HUY THAO TAC.";
                        Sleep(2000);
                    }
                }
            }
        }
        delete[] dsGoiY;
    } 
    SetColor(15);
    return true;    
}

int SaveFileDSSV (PTRSV &First, char *tenfile){
	FILE *f = fopen(tenfile, "w");
	if(f == NULL) return 0;
	for(PTRSV p=First; p!=NULL; p=p->next){
        fprintf(f,"%s|%s|%s|%s|%s|%s\n",
       			p->sv.MASV,
                p->sv.HO,
                p->sv.TEN,
                p->sv.PHAI,
                p->sv.SODT,
                p->sv.MALOP
            );
        }
	fclose(f);
	return 1;  
}

int InsertMH(treeMH &t, MonHoc x){
    if(t == NULL){
        t = new nodeMH;
        t->mh = x;
        t->left = t->right = NULL;
        return 1;
    }
    int kq = strcmp(x.MAMH, t->mh.MAMH);
    if(kq == 0) return 0;  
    if(kq < 0) return InsertMH(t->left, x);
    else return InsertMH(t->right, x);
}

void SaveMH(treeMH t, FILE *f){
    if(t == NULL) return;
    SaveMH(t->left, f);
    fprintf(f, "%s|%s|%d|%d\n",
        t->mh.MAMH,
        t->mh.TENMH,
        t->mh.STCLT,
        t->mh.STCTH
    );
    SaveMH(t->right, f);
}

int SaveFileDSMH(treeMH root, char *tenfile){
    FILE *f = fopen(tenfile, "w");
    if(f == NULL){
        cout << "Khong mo duoc file!\n";
        return 0;
    }
    SaveMH(root, f);
    fclose(f);
    return 1;
}

void FreeTree(treeMH &t){
    if(t == NULL) return;
    FreeTree(t->left);
    FreeTree(t->right);
    delete t;
    t = NULL;
}

int OpenFileDSMH(treeMH &root, char *tenfile){
    FILE *f = fopen(tenfile, "r");
    if(f == NULL){
        cout << "Khong mo duoc file!\n";
        return 0;
    }
    FreeTree(root);
	MonHoc mh;
    while(fscanf(f, "%[^|]|%[^|]|%d|%d\n",
        mh.MAMH,
        mh.TENMH,
        &mh.STCLT,
        &mh.STCTH) == 4){
        InsertMH(root, mh); 
    }
    fclose(f);
    return 1;
}

void InsertLast(PTRSV &First, SinhVien sv) {
	PTRSV p = new nodeSV ;
	p->sv = sv; 
    p->next = NULL;
	if(First == NULL) First = p;
	else{ 
        PTRSV Last;
		for(Last = First; Last->next != NULL; Last=Last->next) ;		
		Last->next = p;
	}
}

void InsertLast_DK(PTRDK &First, DangKy dk) {
    PTRDK p = new nodeDK;
    p->dk = dk;
    p->next = NULL;

    if (First == NULL) First = p;
    else {
        PTRDK Last;
        for (Last = First; Last->next != NULL; Last = Last->next);
        Last->next = p;
    }
}

int OpenFileDSSV (PTRSV &First, char *tenfile) {
	PTRSV p;  SinhVien sv ;
	FILE *f = fopen (tenfile, "r");
	if (f == NULL) return 0; 
	while(First != NULL){
		p = First; 
        First = p->next; 
        delete p;
	}
	while(fscanf(f," %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
            sv.MASV,
            sv.HO,
            sv.TEN,
            sv.PHAI,
            sv.SODT,
            sv.MALOP) == 6)
        InsertLast (First , sv); 

	fclose (f);
	return 1;  
}

int SaveFileDSLTC (DSLTC ds, char *tenfile) {
	FILE *f = fopen (tenfile, "w");
	if (f==NULL) return 0;
    for (int i = 0; i < ds.n; i++){
        int so_dk = 0;
        for (PTRDK p = ds.nodes[i]->dssvdk; p != NULL; p = p->next) {
            if (!p->dk.huydangky) { 
                so_dk++;
            }
        }
        
        fprintf(f,"%d|%s|%s|%d|%d|%d|%d|%d|%d\n",
            ds.nodes[i]->MALOPTC,
            ds.nodes[i]->MAMH,
            ds.nodes[i]->NienKhoa,
            ds.nodes[i]->Hocky,
            ds.nodes[i]->Nhom,
            ds.nodes[i]->sosvmin,
            ds.nodes[i]->sosvmax,
            ds.nodes[i]->huylop,
            so_dk);
        for (PTRDK p = ds.nodes[i]->dssvdk; p != NULL; p = p->next) 
		{
        	if (!p->dk.huydangky)
			{
        		fprintf(f, "%s|%f|%d\n", p->dk.MASV, p->dk.DIEM, p->dk.huydangky);
        	}
    	}	
    }
	fclose (f);
	return 1; 
}

int OpenFileDSLTC(DSLTC &ds, char *tenfile){
    FILE *f = fopen(tenfile,"r");
    if(f == NULL){ 
        cout << "Mo file THAT BAI!\n";
	    return 0; 
    }  

    LopTinChi ltc;
    ds.n = 0;     //xoa ds cu
    int so_dk=0;
    while(fscanf(f,"%d|%[^|]|%[^|]|%d|%d|%d|%d|%d|%d\n",
        &ltc.MALOPTC,
        ltc.MAMH,
        ltc.NienKhoa,
        &ltc.Hocky,
        &ltc.Nhom,
        &ltc.sosvmin,
        &ltc.sosvmax,
        &ltc.huylop,
        &so_dk) == 9){
        LopTinChi *p = new LopTinChi;
        *p = ltc;
        // =========doc sinh vien======
        p->dssvdk = NULL;
        for(int k = 0; k < so_dk; k++) {
            DangKy dk;
            fscanf(f, "%[^|]|%f|%d\n", dk.MASV, &dk.DIEM, &dk.huydangky);
            InsertLast_DK(p->dssvdk, dk); 
        }

        ds.nodes[ds.n] = p;
        ds.n++;
    }
    fclose(f);
    return 1;
}

int DemTongSoLopKhongTrung(PTRSV FirstSV) {
    int nTotalLop = 0;
    char dsLopTam[500][50]; 
    
    for (PTRSV p = FirstSV; p != NULL; p = p->next) 
    {
        bool daTonTai = false;
        for (int i = 0; i < nTotalLop; i++) 
        {
            if (strcmp(dsLopTam[i], p->sv.MALOP) == 0) 
            {
                daTonTai = true;
                break;
            }
        }
        if (!daTonTai && nTotalLop < 500) 
        { 
            strcpy(dsLopTam[nTotalLop], p->sv.MALOP);
            nTotalLop++;
        }
    }
    return nTotalLop;
}

void LayDanhSachCacTenLop(PTRSV FirstSV, char dsLop[][50], int &nTotalLop) {
    nTotalLop = 0;
    for (PTRSV p = FirstSV; p != NULL; p = p->next) 
	{
        bool daTonTai = false;
        for (int i = 0; i < nTotalLop; i++) 
		{
            if (strcmp(dsLop[i], p->sv.MALOP) == 0) 
			{
                daTonTai = true;
                break;
            }
        }
        if (!daTonTai) 
		{ 
            strcpy(dsLop[nTotalLop++], p->sv.MALOP);
        }
    }
}


void VeKhungGoiY_Lop(char dsGoiY[][50], int nGoiY, int pointer, int startX, int startY) 
{
    int boardX = startX;
    int boardY = startY + 5; 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int trangHienTai = pointer / 8;
    int tongSoTrang = (nGoiY == 0) ? 1 : (nGoiY - 1) / 8 + 1;
	SetColor(11); 
    gotoxy(boardX, boardY);
    cout << "| " << left << setw(10) << "STT" << " | " << setw(48) << "MA LOP" << " |";
    gotoxy(boardX, boardY + 1);
    cout << "|------------|--------------------------------------------------|";
	int batDau = trangHienTai * 8;
    for (int i = 0; i < 8; i++) 
	{
        int idx = batDau + i;
        gotoxy(boardX, boardY + 2 + i);
		if (idx < nGoiY) 
		{
           if (idx == pointer)
			{
			    SetConsoleTextAttribute(hConsole, 79); 
			    cout << "|" << " " << left << setw(10) << idx + 1 << " | " << setw(48) << dsGoiY[idx] << " " << "|";
			    SetConsoleTextAttribute(hConsole, 7); 
			} 
			else 
			{
                SetColor(11); cout << "|";
                SetColor(15);
                cout << " " << left << setw(10) << idx + 1 << " ";
                SetColor(11); cout << "|";
                SetColor(15);
                cout << " " << left << setw(48) << dsGoiY[idx] << " ";
                SetColor(11); cout << "|";
            }
        } 
		else 
		{
            SetColor(11);
            cout << "|            |                                                  |";
        }
    }
    SetColor(11);
    gotoxy(boardX, boardY + 10);
    cout << "=================================================================";
    SetColor(14); 
    gotoxy(boardX + 25, boardY + 11);
    cout << "TRANG " << trangHienTai + 1 << " / " << tongSoTrang;
}

void SapXepTheoTenHo(PTRSV FirstSV, char malop[], PTRSV arr[], int &n) 
{
    n = 0;
    for (PTRSV p = FirstSV; p != NULL; p = p->next) 
	{
        if (strcmp(p->sv.MALOP, malop) == 0) arr[n++] = p;
    }
    for (int i = 0; i < n - 1; i++) 
	{
        for (int j = i + 1; j < n; j++) 
		{
            if (strcmp(arr[i]->sv.TEN, arr[j]->sv.TEN) > 0 || (strcmp(arr[i]->sv.TEN, arr[j]->sv.TEN) == 0 && strcmp(arr[i]->sv.HO, arr[j]->sv.HO) > 0)) 
            {
                PTRSV temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
        }
    }
}

void HienThiKetQuaIn_TenHo(PTRSV arr[], int n, char malop[]) 
{
    int trang = 0, soDong = 10;
    int tongTrang = (n == 0) ? 1 : (n - 1) / soDong + 1;
    system("cls");
    int screenW, screenH; GetConsoleSize(screenW, screenH);
    int startX = (screenW - 95) / 2, startY = 2;
    bool loadLaiKhung = true;
	while (true) 
	{
        if (loadLaiKhung) 
		{
            SetColor(11); 
            gotoxy(startX, startY);     cout << "________________________________________________________________________________________________";
            gotoxy(startX, startY + 1); cout << "| STT |      MASV      |                  HO                |    TEN    |  PHAI  |     SODT    |";
            gotoxy(startX, startY + 2); cout << "|-----|----------------|------------------------------------|-----------|--------|-------------|";
            int dayY = startY + 3 + soDong;
            gotoxy(startX, dayY);       cout << "|_____|________________|____________________________________|___________|________|_____________|";
        	SetColor(14); 
            gotoxy(startX, dayY + 2);	cout << "                      TRANG: " << setw(2) << trang + 1 << " / " << setw(2) << tongTrang << "      LOP: " << left << setw(15) << malop << "SI SO: " << n << " SV";
            SetColor(7); gotoxy(startX, dayY + 4); 
            							cout << "[<- / ->]: Sang trang                                                 [ESC]: Quay lai chon lop";
            loadLaiKhung = false;
        }
		AnConTro(true);
        int batDau = trang * soDong, ketThuc = (batDau + soDong > n) ? n : batDau + soDong;
		for (int i = 0; i < soDong; i++) 
		{
            int rowY = startY + 3 + i;
            int idx = batDau + i;
            gotoxy(startX, rowY); 
            SetColor(11); cout << "|"; 
			if (idx < ketThuc) 
			{
                SetColor(15); 
                cout << " " << right << setw(3) << idx + 1 << " | " 
                     << left  << setw(14) << arr[idx]->sv.MASV << " | "
                     << setw(34) << arr[idx]->sv.HO   << " | "
                     << setw(9)  << arr[idx]->sv.TEN  << " | "
                     << setw(6)  << arr[idx]->sv.PHAI << " | "
                     << setw(11) << arr[idx]->sv.SODT << " ";
            } 
			else 
			{
                SetColor(11); cout << "     |                |                                    |           |        |             ";
            }
            SetColor(11); cout << "|"; 
        }
		int k = getch();
        if (k == 27) return; 
        if (k == 224 || k == 0) 
		{
            k = getch();
            if (k == 77 && (trang + 1) < tongTrang) { trang++; loadLaiKhung = true; }
            if (k == 75 && trang > 0) { trang--; loadLaiKhung = true; }
        }
    }
}

void InDSSV_1lop_TheoHo_Ten(PTRSV FirstSV) 
{
    char input[20] = ""; 
    int nIn = 0, ptrLop = 0; 
    
    while (true) 
	{
        system("cls");
        int screenW, screenH; 
        GetConsoleSize(screenW, screenH);
        int lX = (screenW - 65) / 2, lY = (screenH - 20) / 2;

        int nLop = DemTongSoLopKhongTrung(FirstSV);
	    int sSize = (nLop == 0) ? 1 : nLop;
	    
	    char (*dsLop)[50] = new char[sSize][50];
	    char (*dsGoiY)[50] = new char[sSize][50];
	    
	    int nLopCurrent = 0; 
	    if (nLop > 0) {
	        LayDanhSachCacTenLop(FirstSV, dsLop, nLopCurrent);
	    }

        while (true) 
		{
            int nGoiY = 0;
            for (int i = 0; i < nLop; i++) 
			{
                if (nIn == 0 || strstr(dsLop[i], input)) 
				{
                    strcpy(dsGoiY[nGoiY++], dsLop[i]);
                }
            }
            if (ptrLop >= nGoiY && nGoiY > 0) ptrLop = nGoiY - 1;
            
            AnConTro(true); 
            SetColor(11);
            gotoxy(lX, lY + 0); cout << "=================================================================";
            gotoxy(lX, lY + 1); cout << "|                                                               |";
            gotoxy(lX, lY + 2); cout << "|   NHAP MA LOP CAN IN (TEN+HO):                                |";
            gotoxy(lX, lY + 3); cout << "|                                                               |";
            gotoxy(lX, lY + 4); cout << "======================== DANH SACH LOP ==========================";
            VeKhungGoiY_Lop(dsGoiY, nGoiY, ptrLop, lX, lY);
            SetColor(7); 
            gotoxy(lX, lY + 18); cout << "[Up/Down]: Chon                                      [Enter]: Xem";
            gotoxy(lX, lY + 19); cout << "[<- / ->]: Sang trang                                [ESC]: Thoat.";
            SetColor(15); 
            gotoxy(lX + 33, lY + 2); cout << input;
            AnConTro(false); 
            gotoxy(lX + 33 + nIn, lY + 2);
            int c = getch();
            if (c == 27)
			{
                delete[] dsGoiY;
                delete[] dsLop;
                SetColor(7);
                return; 
            }
            if (c == 0 || (unsigned char)c == 224) 
			{
                c = getch();
                if (c == 72 && ptrLop > 0) ptrLop--; 
                else if (c == 80 && ptrLop < nGoiY - 1) ptrLop++; 
                else if (c == 77) { ptrLop += 8; if (ptrLop >= nGoiY) ptrLop = nGoiY - 1; } 
                else if (c == 75) { ptrLop -= 8; if (ptrLop < 0) ptrLop = 0; } 
            }
            else if (c == 13 && nGoiY > 0) 
			{
                int nSV = 0;
                for (PTRSV p = FirstSV; p != NULL; p = p->next) 
				{
                    if (strcmp(p->sv.MALOP, dsGoiY[ptrLop]) == 0) nSV++;
                }
            	if (nSV == 0) 
				{
                    SetColor(12); 
                    gotoxy(lX, lY + 19); 
                    cout << "Lop nay chua co sinh vien!               "; 
                    Sleep(2000);
                    clearLine(lX, lY + 19, 40);
                } 
                else 
				{
                    PTRSV* arr = new PTRSV[nSV]; 
                    SapXepTheoTenHo(FirstSV, dsGoiY[ptrLop], arr, nSV);
                    HienThiKetQuaIn_TenHo(arr, nSV, dsGoiY[ptrLop]);
                    delete[] arr; 
                    break; 
                }
            }
            else if (c == 8 && nIn > 0) { input[--nIn] = '\0'; ptrLop = 0; }
            else if (isalnum(c) && nIn < 15) { input[nIn++] = toupper(c); input[nIn] = '\0'; ptrLop = 0; }
        }
        
        delete[] dsGoiY;
        delete[] dsLop;
    }
}

bool TrungTenMH(treeMH t, char ten[]){
    if(t == NULL) return false;
	if(strcmp(t->mh.TENMH, ten) == 0)
        return true;
	return TrungTenMH(t->left, ten) || TrungTenMH(t->right, ten);
}

int NhapMH(treeMH &root) {
    MonHoc mh;
    strcpy(mh.MAMH, ""); strcpy(mh.TENMH, "");
    mh.STCLT = 0; mh.STCTH =0 ;
	int column = 0; 
    char ch;
	int startX = 30; 
    int startY = 7;  
	system("cls"); 
	while (true) 
	{
        SetColor(11); 
        gotoxy(startX, startY);      cout << "================= NHAP THONG TIN MON HOC =================\n";
        gotoxy(startX, startY + 1);  cout << "|                                                        |\n"; 
        gotoxy(startX, startY + 2);  cout << "| 1. Ma mon hoc  :                                       |\n";
        gotoxy(startX, startY + 3);  cout << "|                                                        |\n"; 
        gotoxy(startX, startY + 4);  cout << "| 2. Ten mon hoc :                                       |\n";
        gotoxy(startX, startY + 5);  cout << "|                                                        |\n"; 
        gotoxy(startX, startY + 6);  cout << "| 3. TC Ly thuyet:                                       |\n";
        gotoxy(startX, startY + 7);  cout << "|                                                        |\n"; 
        gotoxy(startX, startY + 8);  cout << "| 4. TC Thuc hanh:                                       |\n";
        gotoxy(startX, startY + 9);  cout << "|                                                        |\n"; 
        gotoxy(startX, startY + 10); cout << "==========================================================\n";
        SetColor(7); 
        gotoxy(startX, startY + 11);
        cout << "[Up/Down]: Di chuyen     | [Enter]: Luu    | [ESC]: Thoat";
        gotoxy(startX, startY + 12);
        cout << "----------------------------------------------------------";
		SetColor(7);
        gotoxy(startX + 19, startY + 2); cout << mh.MAMH;
        gotoxy(startX + 19, startY + 4); cout << mh.TENMH;
        if(mh.STCLT != 0 || column > 2) { gotoxy(startX + 19, startY + 6); cout << mh.STCLT; }
        if(mh.STCTH != 0 || column > 3) { gotoxy(startX + 19, startY + 8); cout << mh.STCTH; }
        switch (column) {
            case 0: gotoxy(startX + 19 + strlen(mh.MAMH), startY +  2); break;
            case 1: gotoxy(startX + 19 + strlen(mh.TENMH), startY + 4); break;
            case 2: gotoxy(startX + 19 , startY + 6); break;
            case 3: gotoxy(startX + 19 ,startY + 8); break;
        }
		ch = getch();
		if (ch == -32 || ch == 0) 
		{
            ch = getch();
            if (ch == 72) { column = (column > 0) ? column - 1 : 3; continue; } 
            if (ch == 80) { column = (column < 3) ? column + 1 : 0; continue; } 
            continue;
        }
        if (ch == 27) return 0; 
		if (ch == 13) 
		{
            if (strlen(mh.MAMH) == 0) {
                SetColor(14); gotoxy(startX, startY + 14); cout << "LOI: Ma mon hoc khong duoc de trong!        ";
                Sleep(1000); clearLine(startX, startY + 14, 60); column = 0;
            }  
			else if (strlen(mh.TENMH)==0) {
                SetColor(14); gotoxy(startX, startY + 14); cout << "LOI: Ten mon hoc khong duoc de trong!       ";
                Sleep(1000); clearLine(startX, startY + 14, 60); column = 1;
			}  
			else if (mh.STCLT == 0 && mh.STCTH == 0) {
                SetColor(14); gotoxy(startX, startY + 14); cout << "LOI: STCLT va STCTH khong the cung bang 0!  ";
                Sleep(1000); clearLine(startX, startY + 14, 60); column = 2;
            } 
			else {
                InsertMH(root, mh);
                SetColor(10); 
                gotoxy(startX, startY + 14); cout << "SUCCESS: Them mon hoc thanh cong!            ";
                Sleep(1200); SetColor(7); return 1;
            }
            continue;
        }
		ungetch(ch);
        SetColor(7); 
        if (column == 0) 
		{
            clearLine(startX + 19, startY + 2, 30);
            char *s = nhap_ma();
            strcpy(mh.MAMH, s); delete[] s;
            if (strlen(mh.MAMH) > 0 && TimMH(root, mh.MAMH) != NULL) {
                SetColor(14); gotoxy(startX, startY + 14); cout << "LOI: Ma bi trung! Nhap lai.                 ";
                Sleep(1200); clearLine(startX, startY + 14, 60);
                strcpy(mh.MAMH, ""); clearLine(startX + 19, startY + 2, 30);
            } else column = 1;
        }
        else if (column == 1) {
            clearLine(startX + 19, startY + 4, 30);
            char *b = nhap_chu();
            strcpy(mh.TENMH, b); delete[] b;
            column = 2;
        }
        else if (column == 2) {
            clearLine(startX + 19, startY + 6, 5);
            char *c = nhap_so();
            mh.STCLT = atoi(c); delete[] c;
            column = 3;
        }
        else if (column == 3) {
            clearLine(startX + 19, startY + 8, 5);
            char *d = nhap_so();
            mh.STCTH = atoi(d); delete[] d;
            column = 4; 
            
            if (mh.STCLT == 0 && mh.STCTH == 0) {
                SetColor(14); gotoxy(startX, startY + 14); cout << "LOI: STCLT va STCTH khong the cung bang 0!  ";
                Sleep(1200); clearLine(startX, startY + 14, 60);
                column = 2; 
            }
        }
    }
}

void DuyetTreeLayGoiY(treeMH t, char* input, treeMH dsGoiY[], int &nGoiY) {
    if ( t!= NULL) {
        DuyetTreeLayGoiY(t->left, input, dsGoiY, nGoiY);
        if (strlen(input) == 0 || strstr(t->mh.MAMH, input) != NULL) {
            { 
                dsGoiY[nGoiY] = t;
                nGoiY++;
            }
        }
        DuyetTreeLayGoiY(t->right, input, dsGoiY, nGoiY);
    }
}

void LayGoiY_MaMH(treeMH root, char* input, treeMH dsGoiY[], int &nGoiY) {
    nGoiY = 0;
    DuyetTreeLayGoiY(root, input, dsGoiY, nGoiY);
}
																																		
void VeKhungGoiY_MH(treeMH dsMH[], int nGoiY, int pointer, int trangHienTai, int startX, int startY) {
    int boardX = startX; 
    int boardY = startY + 3;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetColor(11); 
    gotoxy(boardX, boardY); 
    cout << "| " << left << setw(15) << "MA MH"  << " | " << setw(30) << "TEN MON HOC"  << " | " << setw(6)  << "STCLT"  << " | " << setw(6)  << "STCTH" << " |";
    gotoxy(boardX, boardY + 1);
    cout << "|-----------------|--------------------------------|--------|--------|";

    int batDau = trangHienTai * 8; 

    for (int i = 0; i < 8; i++) { 
        int idx = batDau + i; 
        gotoxy(boardX, boardY + 2 + i);
        if (idx < nGoiY) {
            if (idx == pointer) {
                SetConsoleTextAttribute(hConsole, 79); 
                cout << "| " << left << setw(15) << dsMH[idx]->mh.MAMH<< " | " << setw(30) << dsMH[idx]->mh.TENMH << " | " << setw(6)  << dsMH[idx]->mh.STCLT << " | " << setw(6)  << dsMH[idx]->mh.STCTH << " |";
                SetConsoleTextAttribute(hConsole, 7); 
            } else {
                SetColor(11); cout << "| ";
                SetColor(15); cout << left << setw(15) << dsMH[idx]->mh.MAMH;
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(30) << dsMH[idx]->mh.TENMH;
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(6)  << dsMH[idx]->mh.STCLT;
                SetColor(11); cout << " | ";
                SetColor(15); cout << left << setw(6)  << dsMH[idx]->mh.STCTH;
                SetColor(11); cout << " |"; 
            }
        } else {
            SetColor(11);
            cout << "|                 |                                |        |        |"; 
        }
    }
    SetColor(11);
    gotoxy(boardX, boardY + 10); 
    cout << "======================================================================";
    if (nGoiY > 0) {
        SetColor(14); 
        int tongTrang = (nGoiY - 1) / 8 + 1;
        gotoxy(boardX + 19, boardY + 11);
        cout << "--- Trang: " << (trangHienTai + 1) << "/" << tongTrang << " (Tong: " << nGoiY << " MH) ---";
    }
}

bool HieuChinhMH(treeMH &root) {
    int startX = 25;
    int startY = 5;
    char mamh_chon[20] = "";
	while (true) 
	{
        system("cls");
        char input[20] = ""; int nIn = 0; 
        int ptrMH = 0; int trang = 0;
        int n = CountNode(root);
        treeMH* dsGoiY=new treeMH[n]; 
        int nGoiY = 0;
        while (true) 
		{
            LayGoiY_MaMH(root, input, dsGoiY, nGoiY);
            trang = ptrMH / 8; 
			AnConTro(true); 
            SetColor(11);
            gotoxy(startX, startY + 0); cout << "======================================================================";
            gotoxy(startX, startY + 1); cout << "|                                                                    |";
            gotoxy(startX, startY + 2); cout << "|   NHAP MA MH CAN SUA:                                              |";
            gotoxy(startX, startY + 3); cout << "|                                                                    |";
            gotoxy(startX, startY + 4); cout << "======================================================================";
            VeKhungGoiY_MH(dsGoiY, nGoiY, ptrMH, trang, startX, startY+2);
			SetColor(7);
            gotoxy(startX, startY + 18); cout << "[Len/Xuong]: Chon                             [Trai/Phai]: Sang trang";
            gotoxy(startX, startY + 19); cout << "[Enter]: Sua MH                                          [ESC]: Thoat";
			SetColor(15);
            gotoxy(startX + 24, startY + 2); cout << input << "                ";
            AnConTro(false); 
            gotoxy(startX + 24 + nIn, startY + 2); 
			int c = getch();
            if (c == 27) return false;
            if (c == 0 || c == -32 || (unsigned char)c == 224) {
                c = getch();
                if (c == 72 && ptrMH > 0) ptrMH--;          
                else if (c == 80 && ptrMH < nGoiY - 1) ptrMH++; 
                else if (c == 77) { ptrMH += 8; if (ptrMH >= nGoiY) ptrMH = nGoiY - 1; }
                else if (c == 75) { ptrMH -= 8; if (ptrMH < 0) ptrMH = 0; }
            }
            else if (c == 13) {
                if (nGoiY > 0) { 
                    strcpy(mamh_chon, dsGoiY[ptrMH]->mh.MAMH); 
                    break; 
                }
                else if (strlen(input) > 0) { 
                    strcpy(mamh_chon, input); 
                    break; 
                }
            }
            else if (c == 8 && nIn > 0) { input[--nIn] = '\0'; ptrMH = 0; }
            else if (isalnum(c) && nIn < 15) { input[nIn++] = toupper(c); input[nIn] = '\0'; ptrMH = 0; }
        }
        if (strlen(mamh_chon) > 0) {
            treeMH p = TimMH(root, mamh_chon);
			if (p == NULL) {
                continue; 
            }
            system("cls");
            int column = 0; 
            char ch;
            bool quayLaiGoiY = false;
            while (true) 
			{
                SetColor(11);
                gotoxy(startX, startY);      cout << "================== HIEU CHINH MON HOC =================\n";
                gotoxy(startX, startY + 1);  cout << "|                                                     |\n";
                gotoxy(startX, startY + 2);  cout << "| 1. Ma mon hoc  : " << p->mh.MAMH; 
                gotoxy(startX + 54, startY + 2); cout << "|\n";
                gotoxy(startX, startY + 3);  cout << "|                                                     |\n";
                gotoxy(startX, startY + 4);  cout << "| 2. Ten mon hoc :                                    |\n";
                gotoxy(startX, startY + 5);  cout << "|                                                     |\n";
                gotoxy(startX, startY + 6);  cout << "| 3. TC Ly thuyet:                                    |\n";
                gotoxy(startX, startY + 7);  cout << "|                                                     |\n";
                gotoxy(startX, startY + 8);  cout << "| 4. TC Thuc hanh:                                    |\n";
                gotoxy(startX, startY + 9);  cout << "|                                                     |\n";
                gotoxy(startX, startY + 10); cout << "=======================================================\n";
				SetColor(7);
                gotoxy(startX, startY + 11);
                cout << "[Up/Down]: Di chuyen  | [Enter]: Cap nhat  | [ESC]: Huy";
                gotoxy(startX, startY + 12);
                cout << "-------------------------------------------------------";
                SetColor(7);
                gotoxy(startX + 19, startY + 4); cout << p->mh.TENMH;
                gotoxy(startX + 19, startY + 6); cout << p->mh.STCLT;
                gotoxy(startX + 19, startY + 8); cout << p->mh.STCTH;
                switch (column) {
                    case 0: gotoxy(startX + 19 + strlen(p->mh.TENMH), startY + 4); break;
                    case 1: gotoxy(startX + 19 + (p->mh.STCLT > 9 ? 2 : 1), startY + 6); break;
                    case 2: gotoxy(startX + 19 + (p->mh.STCTH > 9 ? 2 : 1), startY + 8); break;
                }
				ch = getch();
				if (ch == 27) {   delete[] dsGoiY; quayLaiGoiY = true; break; } 
				if (ch == -32 || ch == 0 || (unsigned char)ch == 224) {
                    ch = getch();
                    if (ch == 72) { column = (column > 0) ? column - 1 : 2; continue; }
                    if (ch == 80) { column = (column < 2) ? column + 1 : 0; continue; }
                    continue;
                }
				if (ch == 13) 
				{
                    if (p->mh.STCLT == 0 && p->mh.STCTH == 0) {
                        SetColor(14); gotoxy(startX, startY + 14);
                        cout << "LOI: STCLT va STCTH khong the cung bang 0!  ";
                        Sleep(1200); clearLine(startX, startY + 14, 50);
                        column = 1;
                    } else {
                        SetColor(10); gotoxy(startX, startY + 14);
                        cout << "SUCCESS: Cap nhat mon hoc thanh cong!       ";
                        Sleep(1200);
                        break; 
                    }
                    continue;
                }
				ungetch(ch);
                SetColor(7);
                if (column == 0) {
                    clearLine(startX + 19, startY + 4, 30);
                    char *b = nhap_chu();
                    if (strlen(b) > 0) strcpy(p->mh.TENMH, b);
                    delete[] b;
                    column = 1;
                }
                else if (column == 1) {
                    clearLine(startX + 19, startY + 6, 5);
                    char *c = nhap_so();
                    if (strlen(c) > 0) p->mh.STCLT = atoi(c);
                    delete[] c;
                    column = 2;
                }
                else if (column == 2) {
                    clearLine(startX + 19, startY + 8, 5);
                    char *d = nhap_so();
                    if (strlen(d) > 0) p->mh.STCTH = atoi(d);
                    delete[] d;
                }
            }
            if (quayLaiGoiY) continue;
        }
        delete[] dsGoiY;
    }
  
    return true;
}

void TimNodeTheMang(treeMH &x, treeMH &y){
    if(y->left != NULL)
        TimNodeTheMang(x,y->left);
    else
	{
        x->mh = y->mh;
        x = y;
        y = y->right;
    }
}

int XoaNodeTrenCay(treeMH &t, char* mamh) {
    if (t == NULL) return 0;
	int kq = strcmp(mamh, t->mh.MAMH);
    if (kq < 0) return XoaNodeTrenCay(t->left, mamh);
    if (kq > 0) return XoaNodeTrenCay(t->right, mamh);
    nodeMH* p = t;
    if (t->left == NULL) t = t->right;
    else if (t->right == NULL) t = t->left;
    else TimNodeTheMang(p, t->right);
	delete p;
    return 1;
}

void VeKhungGoiY_Full(treeMH ds[], int nGoiY, int pointer, int trangHienTai, int startX, int startY) {
    int boardX = startX; 
    int boardY = startY + 5; 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetColor(11);
    gotoxy(boardX, boardY);     cout << "==========================================================";
    gotoxy(boardX, boardY + 1); cout << "|   MA MH    |        TEN MON HOC         | STC (LT/TH)  |";
    gotoxy(boardX, boardY + 2); cout << "|------------|----------------------------|--------------|";
	int batDau = trangHienTai * 8; 
    for (int i = 0; i < 8; i++) 
	{ 
        int idx = batDau + i; 
        gotoxy(boardX, boardY + 3 + i);
        if (idx < nGoiY) 
		{
            if (idx == pointer) {
                SetConsoleTextAttribute(hConsole, 79); 
				cout << "|  " << left << setw(10) << ds[idx]->mh.MAMH<< "|  " << left << setw(26) << ds[idx]->mh.TENMH<< "|  " << ds[idx]->mh.STCLT << "/" << left << setw(10) << ds[idx]->mh.STCTH<< "|";
    			SetConsoleTextAttribute(hConsole, 7); 
            } else 
			{
				SetColor(11); cout << "| ";SetColor(15); cout << left << setw(10) << ds[idx]->mh.MAMH;SetColor(11); cout << " | ";
				SetColor(15); cout << left << setw(26) << ds[idx]->mh.TENMH;SetColor(11); cout << " | ";SetColor(15); 
				cout << right << setw(2) << ds[idx]->mh.STCLT << "/" << left << setw(8) << ds[idx]->mh.STCTH;SetColor(11); cout << "  |";
            }
        } else {
            SetColor(11); cout << "|            |                            |              |"; 
        }
    }
    gotoxy(boardX, boardY + 11); cout << "==========================================================";
    SetColor(15); 
    gotoxy(boardX, boardY + 12);
    cout << "----------------------------------------------------------";
    SetColor(14); 
    gotoxy(boardX+13, boardY + 13);
    int tongTrang = (nGoiY > 0) ? (nGoiY - 1) / 8 + 1 : 1;
    cout << " Trang: " << right << setw(2) << trangHienTai + 1 << "/" << left << setw(2) << tongTrang << " (Tong so: " << nGoiY << " mon)";
    SetColor(7); 
    gotoxy(boardX, boardY + 15);
    cout << "[Up/Down]: Len/Xuong               [Trai/Phai]: Sang trang";
    gotoxy(boardX, boardY + 16);
    cout << "[Enter]: Chon de xoa                          [ESC]: Thoat";
}

int XoaMH(DSLTC &ds, treeMH &root) {
    char mamh_chon[15] = "";
    int screenW, screenH; GetConsoleSize(screenW, screenH);
	while (true) 
	{
        system("cls");
        int lX = (screenW - 60) / 2, lY = (screenH - 28) / 2;
		char input[15] = ""; int nIn = 0;
        int ptrMH = 0; int trang = 0;
        int n = CountNode(root);
        treeMH *dsGoiY=new treeMH[n];
        int nGoiY = 0;
        while (true) 
		{
            nGoiY = 0;
            LayGoiY_MaMH(root, input, dsGoiY, nGoiY);
            trang = ptrMH / 8;
			AnConTro(true);
            SetColor(11);
            gotoxy(lX, lY + 0); cout << "====================== XOA MON HOC =======================";
            gotoxy(lX, lY + 1); cout << "|                                                        |";
            gotoxy(lX, lY + 2); cout << "|   NHAP MA MH CAN XOA:                                  |";
            gotoxy(lX, lY + 3); cout << "|                                                        |";
            gotoxy(lX, lY + 4); cout << "==========================================================";
            VeKhungGoiY_Full(dsGoiY, nGoiY, ptrMH, trang, lX, lY); 
			SetColor(15);
            gotoxy(lX + 24, lY + 2); cout << input << "               ";
			AnConTro(false);
            gotoxy(lX + 24 + nIn, lY + 2);
			int c = getch();
            if (c == 27) {
            	delete []dsGoiY;
				return 0;
				}
            if (c == 0 || (unsigned char)c == 224) {
                c = getch();
                if (c == 72 && ptrMH > 0) ptrMH--;
                else if (c == 80 && ptrMH < nGoiY - 1) ptrMH++;
                else if (c == 77) { ptrMH += 8; if (ptrMH >= nGoiY) ptrMH = nGoiY - 1; }
                else if (c == 75) { ptrMH -= 8; if (ptrMH < 0) ptrMH = 0; }
            }
            else if (c == 13) {
                if (nGoiY > 0) { strcpy(mamh_chon, dsGoiY[ptrMH]->mh.MAMH); break; }
                else if (strlen(input) > 0) { strcpy(mamh_chon, input); break; }
            }
            else if (c == 8 && nIn > 0) { input[--nIn] = '\0'; ptrMH = 0; }
            else if (isalnum(c) && nIn < 10) { input[nIn++] = toupper(c); input[nIn] = '\0'; ptrMH = 0; }
        }
        if (strlen(mamh_chon) > 0) {
            treeMH p = TimMH(root, mamh_chon);
            if (p != NULL) {
                bool coTheXoa = true;
                for (int i = 0; i < ds.n; i++) {
                    if (strcmp(mamh_chon, ds.nodes[i]->MAMH) == 0 && ds.nodes[i]->huylop == false) {
                        SetColor(14);
                        gotoxy(lX, lY + 23); cout << "=> LOI: Mon hoc dang co lop mo! Khong the xoa.";
                        Sleep(2000); coTheXoa = false; break;
                    }
                }
				if (coTheXoa) {
                    int boxX = lX + 5, boxY = lY + 22;
                    SetColor(13); 
                    gotoxy(boxX, boxY + 0); cout << "_________________________________________";
                    gotoxy(boxX, boxY + 1); cout << "|  BAN CO CHAC CHAN MUON XOA KHONG?     |";
                    gotoxy(boxX, boxY + 2); cout << "|  MH: " << p->mh.TENMH; 
                    gotoxy(boxX + 40, boxY + 2); cout << "|";
                    gotoxy(boxX, boxY + 3); cout << "|  Nhap lua chon cua ban (Y/N): _       |"; 
                    gotoxy(boxX, boxY + 4); cout << "|_______________________________________|";
					gotoxy(boxX + 32, boxY + 3); 
                    char xacNhan = getch();
                    cout << (char)toupper(xacNhan); 
					if (xacNhan == 'y' || xacNhan == 'Y') 
					{
                        if (XoaNodeTrenCay(root, mamh_chon)) {
                            SetColor(10);
                            getch();
                            gotoxy(boxX + 5, boxY + 5); cout << "SUCCESS: DA XOA THANH CONG!";
                            Sleep(1200);
                            
                        }
                    } 
					else 
					{
                        SetColor(14);
                        getch();
                        gotoxy(boxX + 5, boxY + 5); cout << "DA HUY THAO TAC.";
                        Sleep(800);
                        
                    }
                }
            }  
        }
        delete[] dsGoiY;
    }
	return 1;
}

int DemSV_DK(PTRDK First){
    int dem = 0;
    PTRDK p = First;
    while (p != NULL) {
        if (p->dk.huydangky == false){  
            dem++;
        }
        p = p->next;
    }
    return dem;
}


bool LietKeDSLTC(DSLTC ds, treeMH rootMH) 
{
    if (ds.n == 0) return false;
	char locNK[15] = ""; 
    char locHK[2] = "";  
    int oFocus = 0;        
    int soDongTrang = 12;
    int trangHienTai = 0;  
    int* dsIndexLoc = new int[ds.n];
    bool reDrawFrame = true; 
    bool reLoadData = true;  
	int screenW, screenH; 
    GetConsoleSize(screenW, screenH);
    int lX = (screenW - 86) / 2; 
    int lY = (screenH - 22) / 2;
	while (true) 
	{
        NhanDienConTro(false);
		int nLoc = 0;
        int hkInt = (strlen(locHK) > 0) ? (locHK[0] - '0') : 0;
        for (int i = 0; i < ds.n; i++) 
		{
            bool checkNK = (strlen(locNK) == 0 || strstr(ds.nodes[i]->NienKhoa, locNK) != NULL);
            bool checkHK = (hkInt == 0 || ds.nodes[i]->Hocky == hkInt);
            if (checkNK && checkHK) 
			{
                dsIndexLoc[nLoc++] = i;
            }
        }
		int tongTrang = (nLoc > 0) ? (nLoc - 1) / soDongTrang + 1 : 1;
        if (trangHienTai >= tongTrang) trangHienTai = tongTrang - 1;
        if (trangHienTai < 0) trangHienTai = 0;
		if (reDrawFrame) 
		{
            system("cls");
            SetColor(11);
            gotoxy(lX, lY);     cout << "================================= DANH SACH LOP TIN CHI ==============================";
            gotoxy(lX, lY + 1); cout << "|                                                                                    |";
            gotoxy(lX, lY + 2); cout << "|   NIEN KHOA:                  HOC KY:                                              |";
            gotoxy(lX, lY + 3); cout << "|                                                                                    |";
            gotoxy(lX, lY + 4); cout << "======================================================================================";
            gotoxy(lX, lY + 5); cout << "| STT | MA LTC |  MA MON  | TEN MON HOC                | NHOM | HK | NIEN KHOA | HUY |";
            gotoxy(lX, lY + 6); cout << "|-----|--------|----------|----------------------------|------|----|-----------|-----|";
            for(int i = 0; i < soDongTrang; i++) {
                gotoxy(lX, lY + 7 + i); 
								cout << "|     |        |          |                            |      |    |           |     |";
            }
            gotoxy(lX, lY + 7 + soDongTrang); 
								cout << "======================================================================================";
            SetColor(15);  
            gotoxy(lX, lY + 21);cout << "[TAB]: Nhap HK/NK              [<- / ->]: Chuyen trang                    [ESC]: Thoat";
            reDrawFrame = false;
        }
		gotoxy(lX + 15, lY + 2); cout << "              ";
        gotoxy(lX + 39, lY + 2); cout << "     ";
        SetColor(14); 
        gotoxy(lX + 15, lY + 2); cout << left << setw(15) << (strlen(locNK) > 0 ? locNK : "TAT CA");
        gotoxy(lX + 39, lY + 2); cout << left << setw(5)  << (strlen(locHK) > 0 ? locHK : "ALL");
		if (reLoadData) 
		{
            int batDauRow = trangHienTai * soDongTrang;
			for (int i = 0; i < soDongTrang; i++) 
			{
                int idxThuc = batDauRow + i;
                gotoxy(lX, lY + 7 + i); 
                if (idxThuc < nLoc) 
				{
                    int rIdx = dsIndexLoc[idxThuc];
                    treeMH pMH = TimMH(rootMH, ds.nodes[rIdx]->MAMH);

                    SetColor(15); 
                    gotoxy(lX + 2, lY + 7 + i);  cout << left << setw(3)  << idxThuc + 1;
                    gotoxy(lX + 8, lY + 7 + i);  cout << left << setw(6)  << ds.nodes[rIdx]->MALOPTC;
                    gotoxy(lX + 17, lY + 7 + i); cout << left << setw(8)  << ds.nodes[rIdx]->MAMH;
                    gotoxy(lX + 28, lY + 7 + i); cout << left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A");
                    gotoxy(lX + 57, lY + 7 + i); cout << left << setw(4)  << ds.nodes[rIdx]->Nhom;
                    gotoxy(lX + 64, lY + 7 + i); cout << left << setw(2)  << ds.nodes[rIdx]->Hocky;
                    gotoxy(lX + 69, lY + 7 + i); cout << left << setw(9)  << ds.nodes[rIdx]->NienKhoa;
                    gotoxy(lX + 81, lY + 7 + i); 
                    if (ds.nodes[rIdx]->huylop) { SetColor(12); cout << "YES"; }
                    else { SetColor(10); cout << "NO "; }
                    SetColor(7);
                } 
				else 
				{
                    SetColor(11); gotoxy(lX, lY + 7 + i); 
                    cout << "|     |        |          |                            |      |    |           |     |";
                }
            }
            SetColor(14); gotoxy(lX + 25, lY + 7 + soDongTrang + 1); 
            cout << "--- Trang: " << (nLoc > 0 ? (trangHienTai + 1) : 0) << "/" << tongTrang << " (Tong so: " << nLoc << " Lop) ---   ";
            reLoadData = false;
        }
		SetColor(15);
        if (oFocus == 0) 
		{
            gotoxy(lX + 15 + (int)strlen(locNK), lY + 2); 
        } 
		else 
		{
            gotoxy(lX + 39 + (int)strlen(locHK), lY + 2);
        }
        NhanDienConTro(true); 
		int c = getch();
        if (c == 27) 
		{   
			break;
		}
        if (c == 9) 
		{
            reLoadData = true;
            oFocus = (oFocus == 0) ? 1 : 0;
            continue;
        }
        if (c == 0 || (unsigned char)c == 224) 
		{
            c = getch();
            reLoadData = true;
            if (c == 75 && trangHienTai > 0) trangHienTai--;          
            else if (c == 77 && trangHienTai < tongTrang - 1) trangHienTai++; 
        }
        else if (c == 8) 
		{ 
            reLoadData = true;
            if (oFocus == 0 && strlen(locNK) > 0) { locNK[strlen(locNK)-1] = '\0'; trangHienTai = 0; }
            else if (oFocus == 1 && strlen(locHK) > 0) { locHK[0] = '\0'; trangHienTai = 0; }
        }
        else { 
            if (oFocus == 0 && strlen(locNK) < 14) 
			{
                if ((c >= '0' && c <= '9') || c == '-' )
				{
                    int len = strlen(locNK);
                    locNK[len] = toupper(c); locNK[len+1] = '\0';
                    trangHienTai = 0; reLoadData = true;
                } 
				else 
				{
                    cout << "\a";
                }
            }
            else if (oFocus == 1 && strlen(locHK) < 1)
			{
            	if (c >= '1' && c <= '3') 
				{
                locHK[0] = c; locHK[1] = '\0';
                trangHienTai = 0; reLoadData = true;
            	} 
				else 
				{
                cout << "\a";							
                }
            }
        }
    }
    NhanDienConTro(true);
    delete[] dsIndexLoc; 
    return true;
}

void InDanhSachRaManHinh(LopTinChi* ltc, PTRSV FirstSV, treeMH rootMH)
{
    if (ltc == NULL) return;
    int soDongTrang = 10;
    int indexDauTrang = 0; 
    int tongSV = 0;
    
    PTRDK temp = ltc->dssvdk;
    while(temp != NULL) 
    { 
        if (temp->dk.huydangky == false) 
        {
            tongSV++; 
        }
        temp = temp->next; 
    }

    system("cls");
    NhanDienConTro(false); 
    treeMH mh = TimMH(rootMH, ltc->MAMH);
    SetColor(14); 
    gotoxy(10, 1); cout << "DANH SACH SINH VIEN DANG KY LOP TIN CHI";
    SetColor(15); 
    gotoxy(10, 2); cout << "Nien khoa : " << ltc->NienKhoa;
    gotoxy(40, 2); cout << "Hoc ky: " << ltc->Hocky;
    gotoxy(10, 3); cout << "Mon hoc   : " << (mh ? mh->mh.TENMH : "N/A");
    gotoxy(10, 4); cout << "Ma mon    : " << ltc->MAMH;
    gotoxy(10, 5); cout << "Nhom      : " << ltc->Nhom;
    gotoxy(10, 6); cout << "Lop       : " << ltc->MALOPTC;
    SetColor(11); 
    gotoxy(5, 8);  cout << "+-----+----------------+---------------------------+----------------+";
    gotoxy(5, 9);  cout << "| STT |      MASV      |          HO TEN           |   TRANG THAI   |";
    gotoxy(5, 10); cout << "+-----+----------------+---------------------------+----------------+";
    
    int currentY = 11;
    SetColor(11);
    gotoxy(5, currentY + 10); cout << "+-----+----------------+---------------------------+----------------+";
    SetColor(15);
    gotoxy(5, currentY + 13); cout << "[LEFT/RIGHT]: Sang trang                        [ESC/ENTER]: Quay lai";

    while (true) 
    {
        int tongTrang = (tongSV + soDongTrang - 1) / soDongTrang; 
        if (tongTrang == 0) tongTrang = 1;
        int trangHienTai = (indexDauTrang / soDongTrang) + 1;

        PTRDK p = ltc->dssvdk; 
        int demHopLe = 0;
        
        // Dịch con trỏ p đến đúng sinh viên HỢP LỆ đầu tiên của trang
        while (p != NULL) 
        {
            if (p->dk.huydangky == false) 
            {
                if (demHopLe == indexDauTrang) break; // Đã tìm thấy điểm bắt đầu
                demHopLe++;
            }
            p = p->next;
        }

        for (int i = 0; i < soDongTrang; i++) 
        {
            gotoxy(5, currentY + i);
            if (p != NULL) 
            {
                PTRSV sv = TimSV(FirstSV, p->dk.MASV);
                string hoTen = (sv != NULL) ? (string(sv->sv.HO) + " " + sv->sv.TEN) : "N/A";
                SetColor(11); cout << "|"; 
                gotoxy(7, currentY + i);  SetColor(11); cout << left << setw(3) << indexDauTrang + i + 1;
                gotoxy(11, currentY + i); SetColor(11); cout << "| "; SetColor(15); cout << left << setw(14) << (sv ? sv->sv.MASV : "N/A");
                gotoxy(28, currentY + i); SetColor(11); cout << "| "; SetColor(15); cout << left << setw(25) << (hoTen.length() > 25 ? hoTen.substr(0, 25) : hoTen);
                gotoxy(56, currentY + i); SetColor(11); cout << "| "; SetColor(15); cout << left << setw(14) << "DANG KY"; 
                SetColor(11); gotoxy(72, currentY + i); cout << " |";
    
                p = p->next;
                while (p != NULL && p->dk.huydangky == true) 
                {
                    p = p->next;
                }
            } 
            else 
            {
                SetColor(11); cout << "|     |                |                           |                |";
            }
        }
        SetColor(14);
        gotoxy(5, currentY + 11); cout << "Trang: " << trangHienTai << " / " << tongTrang << "   ";

        int c = getch();
        if (c == 27 || c == 13) break; 
        if (c == 0 || (unsigned char)c == 224) 
        {
            c = getch();
            if (c == 75 && indexDauTrang >= soDongTrang) indexDauTrang -= soDongTrang; 
            if (c == 77 && indexDauTrang + soDongTrang < tongSV) indexDauTrang += soDongTrang; 
        }
    }
}

bool InSVdkLTC(DSLTC &dsLTC, PTRSV &FirstSV, treeMH rootMH) 
{
    int W, H; GetConsoleSize(W, H);
    int lX = (W - 85) / 2; int lY = 4;
    char locNK[15] = ""; char locHK[2] = ""; 
    int indexThanhSang = 0; int oFocus = 0; int soDongTrang = 10;
    bool reDrawFrame = true; bool reLoadData = true;
    
    while (true) {
        if (reDrawFrame) {
            system("cls"); NhanDienConTro(false); SetColor(11); 
            gotoxy(lX, lY);     cout << "======================= CHON LOP TIN CHI DE IN DANH SACH ==========================";
            gotoxy(lX, lY + 1); cout << "|                                                                                 |";
            gotoxy(lX, lY + 2); cout << "|   NIEN KHOA:                HOC KY:                                             |";
            gotoxy(lX, lY + 3); cout << "|                                                                                 |";
            gotoxy(lX, lY + 4); cout << "===================================================================================";
            gotoxy(lX, lY + 5); cout << "| STT | MA LTC | MA MON     | TEN MON HOC                | NHOM | HK | NIEN KHOA  |";
            gotoxy(lX, lY + 6); cout << "|-----|--------|------------|----------------------------|------|----|----------- |";
            gotoxy(lX, lY + 17);cout << "===================================================================================";
            SetColor(15);
            gotoxy(lX, lY + 19);cout << "[TAB]: Nhap o loc                                                      [ESC]: Thoat."; 
            gotoxy(lX, lY + 20);cout << "[UP/DOWN]: Chon lop | [LEFT/RIGHT]: Sang trang                       [ENTER]: Xem DS"; 
            reDrawFrame = false;
        }
        int* dsIndexLoc = new int[dsLTC.n]; 
        int nLoc = 0;
        int hkInt = (strlen(locHK) > 0) ? (locHK[0] - '0') : 0;
        for (int i = 0; i < dsLTC.n; i++) 
		{
            if (dsLTC.nodes[i] == NULL) continue;
            bool checkNK = (strlen(locNK) == 0 || strstr(dsLTC.nodes[i]->NienKhoa, locNK) != NULL);
            bool checkHK = (hkInt == 0 || dsLTC.nodes[i]->Hocky == hkInt);
            if (checkNK && checkHK && !dsLTC.nodes[i]->huylop) dsIndexLoc[nLoc++] = i;
        }
        
        if (indexThanhSang >= nLoc && nLoc > 0) indexThanhSang = nLoc - 1;
        int trangHienTai = (nLoc == 0) ? 1 : (indexThanhSang / soDongTrang) + 1;
        int batDauRow = (trangHienTai - 1) * soDongTrang;
        int tongTrang = (nLoc + soDongTrang - 1) / soDongTrang; if(tongTrang == 0) tongTrang = 1;
        if (reLoadData) 
		{
            NhanDienConTro(false);
            SetColor(15);
            gotoxy(lX + 15, lY + 2); cout << left << setw(10) << locNK;
            gotoxy(lX + 38, lY + 2); cout << left << setw(2) << locHK;
            for (int i = 0; i < soDongTrang; i++) 
			{
                int thuc = batDauRow + i;
                gotoxy(lX, lY + 7 + i);
                if (thuc < nLoc) 
				{
                    int rIdx = dsIndexLoc[thuc];
                    treeMH pMH = TimMH(rootMH, dsLTC.nodes[rIdx]->MAMH);
                    
                    if (thuc == indexThanhSang) 
					{
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79); 
                        cout << "| " << left << setw(3)  << thuc + 1
                             << " | " << left << setw(6)  << dsLTC.nodes[rIdx]->MALOPTC
                             << " | " << left << setw(10) << dsLTC.nodes[rIdx]->MAMH 
                             << " | " << left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A")
                             << " | " << left << setw(4)  << dsLTC.nodes[rIdx]->Nhom
                             << " | " << left << setw(2)  << dsLTC.nodes[rIdx]->Hocky
                             << " | " << left << setw(9)  << string(dsLTC.nodes[rIdx]->NienKhoa).substr(0, 9) << "  |";
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    } 
					else 
					{
                        SetColor(11); cout << "| ";
                        SetColor(15); cout << left << setw(3) << thuc + 1;
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(6) << dsLTC.nodes[rIdx]->MALOPTC;
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(10) << dsLTC.nodes[rIdx]->MAMH;
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A");
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(4) << dsLTC.nodes[rIdx]->Nhom;
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(2) << dsLTC.nodes[rIdx]->Hocky;
                        SetColor(11); cout << " | ";
                        SetColor(15); cout << left << setw(9) << string(dsLTC.nodes[rIdx]->NienKhoa).substr(0, 9);
                        SetColor(11); cout << "  |";
                    }
                }
				else 
				{
                    SetColor(11); cout << "|     |        |            |                            |      |    |            |"; 
                }
            }
            SetColor(14); gotoxy(lX + 35, lY + 18);
            cout << "Trang: " << trangHienTai << " / " << tongTrang << "    ";
            SetColor(15);
            if (oFocus == 0) { gotoxy(lX + 15 + (int)strlen(locNK), lY + 2); }
            else { gotoxy(lX + 38 + (int)strlen(locHK), lY + 2); }
            NhanDienConTro(true);
            reLoadData = false;
        }
        int c = getch();
        if (c == 27) 
		{
            delete[] dsIndexLoc; 
            SetColor(7);
            return false;
        }
        if (c == 9) { oFocus = (oFocus + 1) % 2; reLoadData = true; delete[] dsIndexLoc; continue; }
        if (c == 0 || (unsigned char)c == 224) {
            NhanDienConTro(false); 
            c = getch();
            if (c == 72 && indexThanhSang > 0) { indexThanhSang--; reLoadData = true; } 
            if (c == 80 && indexThanhSang < nLoc - 1) { indexThanhSang++; reLoadData = true; } 
            if (c == 75) { indexThanhSang -= soDongTrang; if (indexThanhSang < 0) indexThanhSang = 0; reLoadData = true; } 
            if (c == 77) { if (indexThanhSang + soDongTrang < nLoc) indexThanhSang += soDongTrang; reLoadData = true; } 
        } 
		else if (c == 13) 
		{ 
            if (nLoc > 0) 
			{
                InDanhSachRaManHinh(dsLTC.nodes[dsIndexLoc[indexThanhSang]], FirstSV, rootMH);
                reDrawFrame = true; reLoadData = true;
            }
        } 
		else if (c == 8) 
		{ 
            if (oFocus == 0 && strlen(locNK) > 0) { locNK[strlen(locNK)-1] = '\0'; indexThanhSang = 0; reLoadData = true; }
            else if (oFocus == 1 && strlen(locHK) > 0) { locHK[strlen(locHK)-1] = '\0'; indexThanhSang = 0; reLoadData = true; }
        } 
		else 
		{
            if (oFocus == 0 && strlen(locNK) < 10) { locNK[strlen(locNK)] = toupper(c); locNK[strlen(locNK)+1] = '\0'; indexThanhSang = 0; reLoadData = true; }
            else if (oFocus == 1 && (c >= '1' && c <= '4')) { locHK[0] = c; locHK[1] = '\0'; indexThanhSang = 0; reLoadData = true; }
        }
        delete[] dsIndexLoc;
    }
}

void VeKhungNhapDiem(PTRDK list[], int n, int pointer, int trang, PTRSV FirstSV, int startX, int startY) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int batDau = trang * 8; 
    int hienThi = 8;

    SetColor(11); 
    gotoxy(startX, startY);     cout << "=====================================================================================";
    gotoxy(startX, startY + 1); cout << "| STT |      MASV       |         HO        |         TEN       |       DIEM        |";
    gotoxy(startX, startY + 2); cout << "|-----|-----------------|-------------------|-------------------|-------------------|";

    for (int i = 0; i < hienThi; i++) {
        int idx = batDau + i;
        gotoxy(startX, startY + 3 + i);

        if (idx < n) {
            PTRSV sv = TimSV(FirstSV, list[idx]->dk.MASV);
            
            if (idx == pointer) {
                SetConsoleTextAttribute(hConsole, 79); 
                
                if (list[idx]->dk.DIEM >= 0) {
                    printf("| %-3d | %-15s | %-17s | %-17s | %-17.2f |",
                        idx + 1,
                        sv ? sv->sv.MASV : "",
                        sv ? sv->sv.HO : "",
                        sv ? sv->sv.TEN : "",
                        list[idx]->dk.DIEM
                    );
                } else {
                    printf("| %-3d | %-15s | %-17s | %-17s | %-17s |",
                        idx + 1,
                        sv ? sv->sv.MASV : "",
                        sv ? sv->sv.HO : "",
                        sv ? sv->sv.TEN : "",
                        ""
                    );
                }
                SetConsoleTextAttribute(hConsole, 7); 
            } 
            else {
                SetColor(11); cout << "| ";
                SetColor(15); printf("%-3d", idx + 1);
                SetColor(11); cout << " | ";
                SetColor(15); printf("%-15s", sv ? sv->sv.MASV : "");
                SetColor(11); cout << " | ";
                SetColor(15); printf("%-17s", sv ? sv->sv.HO : "");
                SetColor(11); cout << " | ";
                SetColor(15); printf("%-17s", sv ? sv->sv.TEN : "");
                SetColor(11); cout << " | ";
                
                if (list[idx]->dk.DIEM >= 0){ 
					SetColor(15); printf("%-17.2f", list[idx]->dk.DIEM); 
				}
                else{ 
					SetColor(15); printf("%-17s", ""); 
				}             
                SetColor(11); cout << " |";
            }
        } else {
            SetColor(11);
            cout << "|     |                 |                   |                   |                   |";
        }
    }

    gotoxy(startX, startY + 11);
    SetColor(11);
    cout << "=====================================================================================";

    int tongTrang = (n > 0) ? (n - 1) / 8 + 1 : 1;
    SetColor(14);
    gotoxy(startX, startY + 12);
    printf("Trang: %d/%d  (Tong: %d SV)", trang + 1, tongTrang, n);

    SetColor(7);
    gotoxy(startX, startY + 13);
    cout << "[Up/Down]: Di chuyen            [Enter]: Nhap diem & luu              [ESC]: Thoat";
}



LopTinChi* ChonLTC_Interface(DSLTC &ds, treeMH rootMH) {
    if (ds.n == 0) return NULL;

    int screenW, screenH;
    GetConsoleSize(screenW, screenH);

    int startX = (screenW - 85) / 2;
    int startY = (screenH - 20) / 2;

    char locNK[15] = "";
    char locHK[2] = "";

    int oFocus = 0; // 0 = NK, 1 = HK, 2 = bang
    int indexThanhSang = 0;
    system("cls");

    while (true) {
        // ================= LOC DU LIEU =================
        int dsIndexLoc[1000];
        int nLoc = 0;

        int hkInt = (strlen(locHK) > 0) ? locHK[0] - '0' : 0;

        for (int i = 0; i < ds.n; i++) {
            bool checkNK =
                (strlen(locNK) == 0 ||
                 strstr(ds.nodes[i]->NienKhoa, locNK) != NULL);

            bool checkHK =
                (hkInt == 0 ||
                 ds.nodes[i]->Hocky == hkInt);

            if (checkNK && checkHK && !ds.nodes[i]->huylop) {
                dsIndexLoc[nLoc++] = i;
            }
        }

        if (indexThanhSang >= nLoc)
            indexThanhSang = max(0, nLoc - 1);
        int trang = indexThanhSang / 10;
        
       // ================= VE KHUNG =================
        SetColor(11);
        gotoxy(startX, startY);
		cout << "========================================================================";
		
		gotoxy(startX, startY + 1);
		cout << "| NIEN KHOA:                   HOC KY:                                 |";
		
		gotoxy(startX, startY + 2);
		cout << "========================================================================";
		
		gotoxy(startX, startY + 3);
		cout << "| STT | NIEN KHOA | HK |   MA MH   |        TEN MON HOC         | NHOM |";
        gotoxy(startX, startY + 4);
        cout << "|-----|-----------|----|-----------|----------------------------|------|";
        SetColor(15);
        gotoxy(startX + 15, startY + 1);
        cout << left << setw(15) << locNK;
        gotoxy(startX + 44, startY + 1);
        cout << left << setw(5) << locHK;
        
        for (int i = 0; i < 10; i++) {
            int thuc = trang * 10 + i;
            gotoxy(startX, startY + 5 + i);

            if (thuc < nLoc) {
                int idx = dsIndexLoc[thuc];
                treeMH mh = TimMH(rootMH, ds.nodes[idx]->MAMH);
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                
                if (thuc == indexThanhSang)
                    SetConsoleTextAttribute(hConsole, 79);
                else
                    SetConsoleTextAttribute(hConsole, 15);

                printf("| %-3d | %-9s | %-2d | %-9s | %-26s | %-4d |",
                    thuc + 1,
                    ds.nodes[idx]->NienKhoa,
                    ds.nodes[idx]->Hocky,
                    ds.nodes[idx]->MAMH,
                    (mh ? mh->mh.TENMH : ""),
                    ds.nodes[idx]->Nhom);

                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "|     |           |    |           |                            |      |";
            }
        }

        gotoxy(startX, startY + 15);
        SetColor(11);
        cout << "========================================================================";

        SetColor(14);

        gotoxy(startX, startY + 16);
        cout << "[TAB]: Chuyen o   [UP/DOWN]: Di chuyen   [ENTER]: Chon   [ESC]: Thoat";
        gotoxy(startX, startY + 17);
        cout << "[<- / ->]: Sang trang";

        gotoxy(startX, startY + 18);
        cout << "Trang: "
             << (trang + 1)
             << "/"
             << max(1, (nLoc - 1) / 10 + 1)
             << "   | Tong: "
             << nLoc
             << " lop";

        if (oFocus == 0) {
            AnConTro(false);
            gotoxy(startX + 15 + strlen(locNK), startY + 1);
        }
        else if (oFocus == 1) {
            AnConTro(false);
            gotoxy(startX + 39 + strlen(locHK), startY + 1);
        }
        else {
            AnConTro(true);
        }
\
        int c = getch();
        if (c == 27)
            return NULL;
        // TAB
        if (c == 9) {
            oFocus = (oFocus + 1) % 3;
            continue;
        }
        // ENTER
        if (c == 13) {
		    if (oFocus != 2) {
		        oFocus = 2;
		        continue;
		    }
		    if (nLoc > 0) {
		        return ds.nodes[dsIndexLoc[indexThanhSang]];
		    }
		}

        if (c == 0 || (unsigned char)c == 224) {
		    c = getch();
		    if (c == 72) {
		        oFocus = 2;
		        if (indexThanhSang > 0)
		            indexThanhSang--;
		    }
		
		    else if (c == 80) {
		        oFocus = 2;	
		        if (indexThanhSang < nLoc - 1)
		            indexThanhSang++;
		    }
		    else if (c == 77) {
		        oFocus = 2;
		        int next = indexThanhSang + 10;
		        if (next < nLoc)
		            indexThanhSang = next;
		        else
		            indexThanhSang = nLoc - 1;
		    }
		    else if (c == 75) {
		        oFocus = 2;
		        int prev = indexThanhSang - 10;
		        if (prev >= 0)
		            indexThanhSang = prev;
		        else
		            indexThanhSang = 0;
		    }
		}
        else if (c == 8) {
            if (oFocus == 0 && strlen(locNK) > 0) {
                locNK[strlen(locNK) - 1] = '\0';
                indexThanhSang = 0;
            }
            else if (oFocus == 1 && strlen(locHK) > 0) {
                locHK[0] = '\0';
                indexThanhSang = 0;
            }
        }

        else {
            if (oFocus == 0 && strlen(locNK) < 14) {
                if (isalnum(c) || c == '-') {
                    int len = strlen(locNK);
                    locNK[len] = toupper(c);
                    locNK[len + 1] = '\0';
                    indexThanhSang = 0;
                }
            }

            else if (oFocus == 1 && strlen(locHK) < 1) {
                if (c >= '1' && c <= '3') {
                    locHK[0] = c;locHK[1] = '\0';
                    indexThanhSang = 0;
                }
            }
        }
    }
}

void NhapSuaDiem(DSLTC &ds, PTRSV FirstSV, treeMH rootMH) {
	const int MAX_DANG_KY = 1000;
	
    system("cls");
    int screenW, screenH;
    GetConsoleSize(screenW, screenH);
    int startX = (screenW - 75) / 2; 
    int startY = (screenH - 15) / 2;

    LopTinChi* ltc = ChonLTC_Interface(ds, rootMH);
    if (ltc == NULL) return; // Thoat neu nhan esc 

    //Loc ds sinh vien dang ky 
    vector<PTRDK> list;
    PTRDK p = ltc->dssvdk;
    while (p != NULL) {
        if (!p->dk.huydangky) list.push_back(p);
        p = p->next;
    }

    int n = list.size();
    if (n == 0) {
        int posY = startY + 15; 

        SetColor(11);
        gotoxy(startX, posY);
        cout << "***********************************************************************";
        
        SetColor(12);
        gotoxy(startX, posY + 1);
        cout << " LOP NAY CHUA CO SINH VIEN DANG KY!";
        
        SetColor(7); 
        gotoxy(startX, posY + 2);
        cout << " (Nhan phim bat ky de quay lai)";
        
        getch();
        return; 
    }

    PTRDK arr[MAX_DANG_KY];
    for (int i = 0; i < n; i++) arr[i] = list[i];

    int pointer = 0;
    int tableWidth = 85; 
    startX = (screenW - tableWidth) / 2;
    startY = (screenH - 20) / 2;

    system("cls");
    AnConTro(true); 

    while (true) {
        int trang = pointer / 8;
        VeKhungNhapDiem(arr, n, pointer, trang, FirstSV, startX, startY);

        int c = getch();
        if (c == 27) break;

        if (c == 0 || (unsigned char)c == 224) {
            c = getch();
            if (c == 72 && pointer > 0) pointer--;
            else if (c == 80 && pointer < n - 1) pointer++;
            else if (c == 77) { pointer += 8; if (pointer >= n) pointer = n - 1; }
            else if (c == 75) { pointer -= 8; if (pointer < 0) pointer = 0; }
        }
        else if (c == 13) { 
            AnConTro(false);             
            gotoxy(startX + 66, startY + 3 + (pointer % 8));
            gotoxy(startX + 66, startY + 3 + (pointer % 8));
			            
            char s[10];
            cin.getline(s, 10);
            
            float d = atof(s);
            // Kiem tra d >= 0 va xu ly truong hop atof tra ve 0 khi nhap chu 
            if ((d > 0 || strcmp(s, "0") == 0) && d <= 10) {
                arr[pointer]->dk.DIEM = d;
            }
            
            AnConTro(true); 
        }
    }
    AnConTro(false); 
}

void VeTraCuuMonHoc(treeMH root, int trang) 
{
    int n = CountNode(root);
    if (n == 0) return;
    treeMH *dsMH = new treeMH[n];
    int idxMH = 0;
    TreeToArray(root, dsMH, idxMH);
    
    int soDong = 12; 
    int tongTrang = (n + soDong - 1) / soDong;
    if (trang > tongTrang) trang = tongTrang;
    if (trang < 1) trang = 1;
    int batDau = (trang - 1) * soDong;
    int xDS = 2; 
    SetColor(11); 
    gotoxy(xDS, 3); cout << "==================== MON HOC (TRA CUU) ==================";
    gotoxy(xDS, 4); cout << "| STT |  MA MON  |           TEN MON HOC                |";
    gotoxy(xDS, 5); cout << "|-----|----------|--------------------------------------|";
	for (int i = 0; i < soDong; i++) 
	{
        int idx = batDau + i;
        gotoxy(xDS, 6 + i);
        SetColor(11); cout << "|";
        if (idx < n) 
		{
            SetColor(15);
            cout << " " << left << setw(4) << idx + 1;
            SetColor(11); cout << "| "; 
            SetColor(15);
            cout << left << setw(9) << dsMH[idx]->mh.MAMH;
            SetColor(11); cout << "| "; 
            SetColor(15);
            cout << left << setw(37) << dsMH[idx]->mh.TENMH; 
            SetColor(11); cout << "|"; 
        } 
		else 
		{
            SetColor(11); cout << "     |          |                                      |";
        }
    }
    gotoxy(xDS, 18); cout << "=========================================================";
    SetColor(15); gotoxy(xDS, 19);
    cout << "Trang: " << trang << "/" << (tongTrang == 0 ? 1 : tongTrang) << "  [<-][->]";
    SetColor(15);
    delete[] dsMH;
}
int NamDauNK(const char nk[])
{
    int nam;
    sscanf(nk, "%d", &nam);
    return nam;
}

bool LaQuaKhu(const char nkNhap[], int hkNhap)
{
    char nkHT[15];
    int hkHT;

    LayNienKhoaHocKyHienTai(nkHT, hkHT);

    int namNhap = NamDauNK(nkNhap);
    int namHT = NamDauNK(nkHT);

    if (namNhap < namHT)
        return true;

    if (namNhap == namHT && hkNhap < hkHT)
        return true;

    return false;
}

int ThemLTC(DSLTC &dsLTC, treeMH &cayMonHoc)
{
    if (dsLTC.n >= MAX_LTC) 
	{
        system("cls");
        SetColor(12);
        cout << "Danh sach lop tin chi day!\n";
        getch();
        return 0;
    }
 	char nienKhoaCoi[15] = "";
	char hocKyStr[5] = "";
	int hocKyCoi = 0;
    char maMon[11] = "";
    char nhom[5] = "";
    char svMin[5] = "";
    char svMax[5] = "";
    int cotX = 63;
    int hangY = 5;
    int column = 0;
    int trangMonHoc = 1;
    char phimChon;
    bool thoatVeContext = false;
    system("cls");
	int xNK = 35;
    int yNK = 8;
	int subColumn = 0; 
    bool hoanThanhKhungPhu = false;
    bool veLaiKhungPhu = true; 
	while (!hoanThanhKhungPhu)
    {
        if (veLaiKhungPhu)
        {
            system("cls");
            NhanDienConTro(false);

            SetColor(11);
            gotoxy(xNK, yNK);     cout << "==========================================";
            gotoxy(xNK, yNK + 1); cout << "|                                        |";
            gotoxy(xNK, yNK + 2); cout << "|           THONG TIN HOC KY             |";
            gotoxy(xNK, yNK + 3); cout << "|                                        |";
            gotoxy(xNK, yNK + 4); cout << "| Nien khoa (VD 2025-2026):              |";
            gotoxy(xNK, yNK + 5); cout << "|                                        |";
            gotoxy(xNK, yNK + 6); cout << "| Hoc ky (1 hoac 2):                     |";
            gotoxy(xNK, yNK + 7); cout << "|                                        |";
            gotoxy(xNK, yNK + 8); cout << "==========================================";
			SetColor(14);
            gotoxy(xNK, yNK + 10);     cout << "[ENTER] Xac nhan";
            gotoxy(xNK + 15, yNK + 10);cout << "                [ESC] Thoat";
			SetColor(15);
            gotoxy(xNK + 27, yNK + 4); cout << nienKhoaCoi;
            gotoxy(xNK + 21, yNK + 6); cout << (hocKyCoi > 0 ? hocKyStr : "");
            veLaiKhungPhu = false; 
        }
		if (subColumn == 0) gotoxy(xNK + 27 + strlen(nienKhoaCoi), yNK + 4);
        else gotoxy(xNK + 21 + strlen(hocKyStr), yNK + 6);
		NhanDienConTro(true);
        char phimChonPhu = getch();
		if (phimChonPhu == 27) 
        {
            SetColor(15);
            return 0;
        }
        if (phimChonPhu == -32 || phimChonPhu == 0) 
        {
            phimChonPhu = getch();
            if (phimChonPhu == 72) { subColumn = (subColumn > 0) ? subColumn - 1 : 1; } 
            if (phimChonPhu == 80) { subColumn = (subColumn < 1) ? subColumn + 1 : 0; } 
            continue; 
        }

        if (phimChonPhu == 13) 
        {
            if (strlen(nienKhoaCoi) == 0) { subColumn = 0; continue; }
            if (strlen(hocKyStr) == 0) { subColumn = 1; continue; }

            int namDau, namCuoi;
            if (sscanf(nienKhoaCoi, "%d-%d", &namDau, &namCuoi) != 2 || namCuoi != namDau + 1)
            {
                SetColor(12); gotoxy(xNK, yNK + 12);
                cout << "LOI: Nien khoa phai dang YYYY-YYYY";
                Sleep(2000);
                strcpy(nienKhoaCoi, "");
                subColumn = 0;
                veLaiKhungPhu = true;
                continue;
            }

            hocKyCoi = atoi(hocKyStr);
            if (hocKyCoi != 1 && hocKyCoi != 2)
            {
                SetColor(12); gotoxy(xNK, yNK + 12);
                cout << "LOI: Hoc ky chi duoc 1 hoac 2!";
                Sleep(2000);
                strcpy(hocKyStr, "");
                hocKyCoi = 0;
                subColumn = 1;
                veLaiKhungPhu = true;
                continue;
            }

            if (LaQuaKhu(nienKhoaCoi, hocKyCoi))
            {
                SetColor(12); gotoxy(xNK, yNK + 12);
                cout << "LOI: Khong duoc mo lop trong qua khu!";
                Sleep(2000);
                
                strcpy(nienKhoaCoi, "");
                strcpy(hocKyStr, "");
                hocKyCoi = 0;
                subColumn = 0;
                veLaiKhungPhu = true;
                continue;
            }

            hoanThanhKhungPhu = true;
            continue;
        }

        ungetch(phimChonPhu);
        SetColor(15);
        if (subColumn == 0) 
        {
            clearLine(xNK + 27, yNK + 4, 13);
            char *s = nhap_ma();
            strcpy(nienKhoaCoi, s); delete[] s;
            fflush(stdin);
            if (strlen(nienKhoaCoi) > 0) subColumn = 1; 
        }
        else 
        {
            clearLine(xNK + 21, yNK + 6, 5);
            char *s = nhap_so();
            strcpy(hocKyStr, s); delete[] s;
            fflush(stdin);
        }
    }

    system("cls");
    
    while (!thoatVeContext) 
	{
        NhanDienConTro(false);
        int maTuDong = (dsLTC.n == 0) ? 1 : dsLTC.nodes[dsLTC.n - 1]->MALOPTC + 1;
        VeTraCuuMonHoc(cayMonHoc, trangMonHoc);
        SetColor(11);
        gotoxy(cotX, hangY);
        cout << "========== THEM LOP TIN CHI MOI ==========";
        gotoxy(cotX, hangY + 1);
        cout << "|";
        SetColor(14);
        cout << " Hoc ky: " << hocKyCoi << " - Nien khoa: " << nienKhoaCoi;
        SetColor(11);
        cout << "       |";
        gotoxy(cotX, hangY + 2);
        cout << "|----------------------------------------|";
        gotoxy(cotX, hangY + 3);
        cout << "|";  SetColor(15);cout<<"0. Ma LTC          : ";SetColor(11);cout  << left << setw(18) << maTuDong << " |";
        gotoxy(cotX, hangY + 4);
        cout << "|";  SetColor(15);cout<<"1. Ma mon hoc      : ";SetColor(11);cout  << left << setw(18) << maMon << " |";
        gotoxy(cotX, hangY + 5);
        cout << "|";  SetColor(15);cout<<"2. Nhom            : ";SetColor(11);cout  << left << setw(18) << nhom << " |";
        gotoxy(cotX, hangY + 6);
        cout << "|";  SetColor(15);cout<<"3. SV Toi thieu    : ";SetColor(11);cout  << left << setw(18) << svMin << " |";
        gotoxy(cotX, hangY + 7);
        cout << "|";  SetColor(15);cout<<"4. SV Toi da       : ";SetColor(11);cout  << left << setw(18) << svMax << " |";
        gotoxy(cotX, hangY + 8);
        cout << "==========================================";
        SetColor(15);
        gotoxy(cotX, hangY + 10);
        cout << "[Up/Down]: Di chuyen";
        gotoxy(cotX, hangY + 11);
        cout << "[Left/Right]: Chuyen trang";
        gotoxy(cotX, hangY + 12);
        SetColor(10);
        cout << "[ENTER]: Luu";
        SetColor(14);cout <<"                  [ESC]: Thoat";
        switch (column) 
		{
            case 0: gotoxy(cotX + 22 + strlen(maMon), hangY + 4); break;
            case 1: gotoxy(cotX + 22+ strlen(nhom), hangY + 5); break;
            case 2: gotoxy(cotX + 22 + strlen(svMin), hangY + 6); break;
            case 3: gotoxy(cotX + 22 + strlen(svMax), hangY + 7); break;
        }
        NhanDienConTro(true);
        phimChon = getch();
       if (phimChon == 27) 
        {
            SetColor(7);
            return ThemLTC(dsLTC, cayMonHoc); 
        }
        if (phimChon == -32 || phimChon == 0) 
		{
            phimChon = getch();
            if (phimChon == 72) { column = (column > 0) ? column - 1 : 3; continue; }
            if (phimChon == 80) { column = (column < 3) ? column + 1 : 0; continue; }
            if (phimChon == 75 && trangMonHoc > 1) { trangMonHoc--; continue; }
            if (phimChon == 77) { trangMonHoc++; continue; }
            continue;
        }
        if (phimChon == 13) 
		{
            if (strlen(maMon) == 0) column = 0;
            else if (strlen(nhom) == 0) column = 1;
            else if (strlen(svMin) == 0) column = 2;
            else if (strlen(svMax) == 0) column = 3;
            else if (atoi(svMax) <= atoi(svMin))
			{
                SetColor(12);
                gotoxy(cotX, hangY + 14);
                cout << "LOI: SV Max phai > SV Min!";
                Sleep(2000);
                clearLine(cotX, hangY + 14, 40);
                column = 3;
            }
            else 
			{
                bool biTrung = false;
                for (int i = 0; i < dsLTC.n; i++) 
				{
                    if (strcmp(dsLTC.nodes[i]->MAMH, maMon) == 0 &&
                        strcmp(dsLTC.nodes[i]->NienKhoa, nienKhoaCoi) == 0 &&
                        dsLTC.nodes[i]->Hocky == hocKyCoi &&
                        dsLTC.nodes[i]->Nhom == atoi(nhom)) 
					{
                        biTrung = true;
                        break;
                    }
                }
                if (biTrung) 
				{
                    SetColor(12);
                    gotoxy(cotX, hangY + 14);
                    cout << "LOI: Lop nay da ton tai!";
                    Sleep(2000);
                    clearLine(cotX, hangY + 14, 40);
                    column = 0;
                }
                else 
				{
                    LopTinChi *p = new LopTinChi;
                    p->MALOPTC = maTuDong;
                    strcpy(p->MAMH, maMon);
                    strcpy(p->NienKhoa, nienKhoaCoi);
                    p->Hocky = hocKyCoi;
                    p->Nhom = atoi(nhom);
                    p->sosvmin = atoi(svMin);
                    p->sosvmax = atoi(svMax);
                    p->huylop = false;
                    p->dssvdk = NULL;
                    dsLTC.nodes[dsLTC.n++] = p;
                    SetColor(10);
                    gotoxy(cotX, hangY + 14);
                    cout << "SUCCESS: LUU THANH CONG!";
                    Sleep(2000);
                    strcpy(maMon, "");
                    strcpy(nhom, "");
                    strcpy(svMin, "");
                    strcpy(svMax, "");
                    column = 0;
                    system("cls");
                }
            }
            continue;
        }
        ungetch(phimChon);
        SetColor(15);
        if (column == 0) 
		{
            clearLine(cotX + 22, hangY + 4, 18);
            char *s = nhap_ma();
            strcpy(maMon, s);
            delete[] s;
            if (strlen(maMon) > 0 && TimMH(cayMonHoc, maMon) == NULL)
			{
                SetColor(12);
                gotoxy(cotX, hangY + 14);
                cout << "LOI: Ma mon khong ton tai!";
                Sleep(2000);
                clearLine(cotX, hangY + 14, 40);
                strcpy(maMon, "");
            }
            else if (strlen(maMon) > 0) column = 1;
        }
        else if (column == 1) 
		{
            clearLine(cotX + 22, hangY + 5, 18);
            char *s = nhap_so();
            strcpy(nhom, s);
            delete[] s;
            if (strlen(nhom) > 0) column = 2;
        }
        else if (column == 2) 
		{
            clearLine(cotX + 22, hangY + 6, 18);
            char *s = nhap_so();
            strcpy(svMin, s);
            delete[] s;
            if (strlen(svMin) > 0) column = 3;
        }
        else if (column == 3) 
		{
            clearLine(cotX + 22, hangY + 7, 18);
            char *s = nhap_so();
            strcpy(svMax, s);
            delete[] s;
        }
    }
    return 1;
}

void InDiemLopDaChon(LopTinChi* ltc, PTRSV FirstSV, treeMH rootMH, int lX, int lY) {
    system("cls");
    treeMH pMH = TimMH(rootMH, ltc->MAMH);
    
    SetColor(14); gotoxy(lX + 35, lY - 2); 
    cout << "BANG DIEM MON HOC: " << (pMH ? pMH->mh.TENMH : "N/A");
    
    SetColor(15); gotoxy(lX+20, lY - 1); 
    cout << "Nien khoa: " << ltc->NienKhoa << " | Hoc ky: " << ltc->Hocky << " | Nhom: " << ltc->Nhom;

    SetColor(11);
    gotoxy(lX, lY);     cout << "=================================================================================";
    gotoxy(lX, lY + 1); cout << "| STT |      MASV      |          HO VA TEN          |      LOP      |   DIEM   |";
    gotoxy(lX, lY + 2); cout << "|-----|----------------|-----------------------------|---------------|----------|";

    int dong = 0;
    for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
        if (p->dk.huydangky) continue;
        PTRSV sv = TimSV(FirstSV, p->dk.MASV);
        
        gotoxy(lX, lY + 3 + dong);
        SetColor(11); cout << "| "; 
        SetColor(15); 
        string hoTen = sv ? (string(sv->sv.HO) + " " + sv->sv.TEN) : "N/A";
        
        cout << left << setw(3)  << dong + 1 << " | "
             << left << setw(14) << p->dk.MASV << " | "
             << left << setw(27) << hoTen.substr(0, 27) << " | "
             << left << setw(13) << (sv ? sv->sv.MALOP : "N/A") << " | "
             << right << setw(8) << fixed << setprecision(1) << p->dk.DIEM << " ";
        
        SetColor(11); cout << "|";
        dong++;
    }
    gotoxy(lX, lY + 3 + dong);
    cout << "=================================================================================";
    SetColor(10); gotoxy(lX, lY + 5 + dong); cout << "=> Bam phim bat ky de quay lai...";
    getch();
}

void InBangDiem(DSLTC &dsLTC, PTRSV &FirstSV, treeMH rootMH) {
	const int MAX_LTC = 1000;
	
    int W, H; GetConsoleSize(W, H);
    int lX = (W - 85) / 2; int lY = 4;
    char locNK[15] = ""; char locHK[2] = ""; 
    int indexThanhSang = 0; int oFocus = 0; 
    int soDongTrang = 10; 
    bool reDrawFrame = true; bool reLoadData = true;

    while (true) {
        int dsIndexLoc[MAX_LTC], nLoc = 0;
        int hkInt = (strlen(locHK) > 0) ? (locHK[0] - '0') : 0;
        for (int i = 0; i < dsLTC.n; i++) {
            if (dsLTC.nodes[i] == NULL) continue;
            bool checkNK = (strlen(locNK) == 0 || strstr(dsLTC.nodes[i]->NienKhoa, locNK) != NULL);
            bool checkHK = (hkInt == 0 || dsLTC.nodes[i]->Hocky == hkInt);
            if (checkNK && checkHK && !dsLTC.nodes[i]->huylop) dsIndexLoc[nLoc++] = i;
        }

        int tongTrang = (nLoc + soDongTrang - 1) / soDongTrang;
        if (tongTrang == 0) tongTrang = 1;
        int trangHienTai = (indexThanhSang / soDongTrang) + 1;
        int batDauRow = (trangHienTai - 1) * soDongTrang;

        if (reDrawFrame) {
            system("cls"); NhanDienConTro(false); 
            SetColor(11); 
            gotoxy(lX, lY);     cout << "======================= CHON LOP TIN CHI DE IN BANG DIEM ==========================";
            gotoxy(lX, lY + 1); cout << "|                                                                                 |";
            gotoxy(lX, lY + 2); cout << "|   NIEN KHOA:                HOC KY:                                             |";
            gotoxy(lX, lY + 3); cout << "|                                                                                 |";
            gotoxy(lX, lY + 4); cout << "===================================================================================";
            gotoxy(lX, lY + 5); cout << "| STT | MA LTC | MA MON     | TEN MON HOC                | NHOM | HK | NIEN KHOA  |";
            gotoxy(lX, lY + 6); cout << "|-----|--------|------------|----------------------------|------|----|----------- |";
            for (int i = 0; i < soDongTrang; i++) {
                gotoxy(lX, lY + 7 + i);
                cout << "|     |        |            |                            |      |    |            |";
            }
            gotoxy(lX, lY + 17);cout << "===================================================================================";
        
            SetColor(15);
            gotoxy(lX, lY + 20); cout <<"TAB : Chuyen o  (NK/HK)                                    [LEFT/RIGHT]: Sang trang";
            gotoxy(lX, lY + 21); cout <<"[ENTER]:Chon lop xem diem           [UP/DOWN]: di chuyen                ESC : Thoat";
            
            reDrawFrame = false;
        }

        if (reLoadData) {
            NhanDienConTro(false);
            SetColor(15);
            gotoxy(lX + 15, lY + 2); cout << left << setw(10) << (strlen(locNK) > 0 ? locNK : "");
            gotoxy(lX + 38, lY + 2); cout << left << setw(2) << (strlen(locHK) > 0 ? locHK : "");

            for (int i = 0; i < soDongTrang; i++) {
                int thuc = batDauRow + i;
                gotoxy(lX, lY + 7 + i);
                if (thuc < nLoc) {
                    int rIdx = dsIndexLoc[thuc];
                    treeMH pMH = TimMH(rootMH, dsLTC.nodes[rIdx]->MAMH);
                    
                    SetColor(11); cout << "|"; 
                    if (thuc == indexThanhSang) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
                    else SetColor(15); 

                    cout << " " << left << setw(3) << thuc + 1 << " | "
                         << left << setw(6) << dsLTC.nodes[rIdx]->MALOPTC << " | "
                         << left << setw(10) << dsLTC.nodes[rIdx]->MAMH << " | "
                         << left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A") << " | "
                         << left << setw(4) << dsLTC.nodes[rIdx]->Nhom << " | "
                         << left << setw(2) << dsLTC.nodes[rIdx]->Hocky << " | "
                         << left << setw(10) << dsLTC.nodes[rIdx]->NienKhoa << " ";SetColor(11); cout << "|";     
						 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                } else {
                    SetColor(11); cout << "|     |        |            |                            |      |    |            |";
                }
            }
            
            SetColor(14); gotoxy(lX + 35, lY + 18);
            cout << "Trang " << trangHienTai << " / " << tongTrang;

            if (oFocus == 0) gotoxy(lX + 15 + (int)strlen(locNK), lY + 2);
            else gotoxy(lX + 38 + (int)strlen(locHK), lY + 2);
            NhanDienConTro(true);
            reLoadData = false;
        }

        int c = getch();
        if (c == 27){
        	SetColor(15);
			return; 
		}
        if (c == 9) { oFocus = (oFocus + 1) % 2; reLoadData = true; continue; } 
        
        if (c == 224) {
            c = getch();
            if (c == 72 && indexThanhSang > 0) {
                indexThanhSang--; reLoadData = true; 
            }
            if (c == 80 && indexThanhSang < nLoc - 1) { 
                indexThanhSang++; reLoadData = true; 
            }
            if (c == 75) { 
                indexThanhSang = max(0, indexThanhSang - soDongTrang);
                reLoadData = true;
            }
            if (c == 77) { 
                indexThanhSang = min(nLoc - 1, indexThanhSang + soDongTrang);
                reLoadData = true;
            }
        } else if (c == 13) {
            if (nLoc > 0) {
                InDiemLopDaChon(dsLTC.nodes[dsIndexLoc[indexThanhSang]], FirstSV, rootMH, lX, lY);
                reDrawFrame = true; reLoadData = true;
            }
        } else if (c == 8) { 
            if (oFocus == 0 && strlen(locNK) > 0) { locNK[strlen(locNK)-1] = '\0'; indexThanhSang = 0; reLoadData = true; }
            else if (oFocus == 1 && strlen(locHK) > 0) { locHK[strlen(locHK)-1] = '\0'; indexThanhSang = 0; reLoadData = true; }
        } else { 
            if (oFocus == 0 && strlen(locNK) < 10) { locNK[strlen(locNK)] = toupper(c); locNK[strlen(locNK)+1] = '\0'; indexThanhSang = 0; reLoadData = true; }
            else if (oFocus == 1 && (c >= '1' && c <= '4')) { locHK[0] = c; locHK[1] = '\0'; indexThanhSang = 0; reLoadData = true; }
        }
    }
}

void ChonLopTongKet(PTRSV FirstSV, char *maLopChon) {
    int W, H; GetConsoleSize(W, H);
    int lX = (W - 65) / 2; int lY = 5;
    char dsLopGoc[500][20]; int nLopGoc = 0;

    //Loc danh sach tat ca cac lop duy nhat
    for (PTRSV p = FirstSV; p != NULL; p = p->next) {
        bool trung = false;
        for (int i = 0; i < nLopGoc; i++) {
            if (strcmp(dsLopGoc[i], p->sv.MALOP) == 0) { trung = true; break; }
        }
        if (!trung && nLopGoc < 500) { strcpy(dsLopGoc[nLopGoc++], p->sv.MALOP); }
    }

    char chuoiTimKiem[20] = ""; 
    int chiSoChon = 0;
    int soDongTrang = 10; 
    bool veLaiKhung = true;

    while (true) {
        //Loc danh sach theo tu khoa
        char dsLoc[500][20];
        int nLoc = 0;
        for (int i = 0; i < nLopGoc; i++) {
            if (strlen(chuoiTimKiem) == 0 || strstr(dsLopGoc[i], chuoiTimKiem) != NULL) {
                strcpy(dsLoc[nLoc++], dsLopGoc[i]);
            }
        }

        int tongTrang = (nLoc + soDongTrang - 1) / soDongTrang;
        if (tongTrang == 0) tongTrang = 1;
        
        if (chiSoChon >= nLoc && nLoc > 0) chiSoChon = nLoc - 1;
        if (nLoc == 0) chiSoChon = 0;

        int trangHienTai = (chiSoChon / soDongTrang) + 1;
        int batDauRow = (trangHienTai - 1) * soDongTrang;

        if (veLaiKhung) {
            system("cls");
            SetColor(11); 
            gotoxy(lX, lY);     cout << "=================== TIM KIEM VA CHON LOP ===================";
            gotoxy(lX, lY + 1); cout << "|  NHAP MA LOP:                                            |";
            gotoxy(lX, lY + 2); cout << "============================================================";
            for (int i = 0; i < 10; i++) {
                gotoxy(lX, lY + 3 + i);
                				cout << "|                                                          |";
            }
            gotoxy(lX, lY + 13); cout << "============================================================";
            SetColor(7); 
            gotoxy(lX, lY + 16); cout << "[Enter]: Chon                                   [ESC]: Thoat";
            gotoxy(lX, lY + 17); cout << "[LEFT/RIGHT]: Sang trang                [UP/DOWN]: Di chuyen";
            veLaiKhung = false;
        }

        NhanDienConTro(false);

        SetColor(14); 
        gotoxy(lX + 16, lY + 1); cout << "                  "; 
        gotoxy(lX + 16, lY + 1); cout << (strlen(chuoiTimKiem) > 0 ? chuoiTimKiem : "");

        SetColor(14);
        gotoxy(lX + 25, lY + 14); cout << "Trang " << trangHienTai << "/" << tongTrang << "   ";

        for (int i = 0; i < soDongTrang; i++) {
            int indexThuc = batDauRow + i; 
            gotoxy(lX + 1, lY + 3 + i);
            SetColor(11); cout << ""; 
            
            if (indexThuc < nLoc) {
                if (indexThuc == chiSoChon) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79); 
                else SetColor(15); 
                cout << "  " << left << setw(56) << dsLoc[indexThuc]; 
            } else {
                SetColor(0); cout << setw(58) << " "; 
            }
            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            SetColor(11); cout << "|"; 
        }

        SetColor(14);
        gotoxy(lX + 16 + (int)strlen(chuoiTimKiem), lY + 1);
        NhanDienConTro(true); 

        int ch = getch();
        if (ch == 27) { strcpy(maLopChon, ""); SetColor(15); return; } 
        if (ch == 13) { if (nLoc > 0) { strcpy(maLopChon, dsLoc[chiSoChon]); return; } }
        
        if (ch == 224) {
            ch = getch();
            if (ch == 72 && chiSoChon > 0) chiSoChon--; // Up
            if (ch == 80 && chiSoChon < nLoc - 1) chiSoChon++; // Down
            
            if (ch == 75) { // Left(trang truoc) 
                chiSoChon = max(0, chiSoChon - soDongTrang);
            }
            if (ch == 77) { // Right(trang sau) 
                if (chiSoChon + soDongTrang < nLoc) chiSoChon += soDongTrang;
                else chiSoChon = nLoc - 1;
            }
        } 
        else if (ch == 8) { // Backspace
            if (strlen(chuoiTimKiem) > 0) {
                chuoiTimKiem[strlen(chuoiTimKiem) - 1] = '\0';
                chiSoChon = 0; // Reset ve dau khi tim kiem thay doi
            }
        }
        else if (isalnum(ch) || ch == '-' || ch == '_') {
            if (strlen(chuoiTimKiem) < 18) {
                int len = strlen(chuoiTimKiem);
                chuoiTimKiem[len] = toupper(ch);
                chuoiTimKiem[len + 1] = '\0';
                chiSoChon = 0; 
            }
        }
    }
}

void InDiemTB_Khoa(PTRSV FirstSV, DSLTC ds, treeMH rootMH) {
    char maLop[20];
    while (true) {
        ChonLopTongKet(FirstSV, maLop);
        if (strlen(maLop) == 0) return;
        bool coSV = false;
        for (PTRSV p = FirstSV; p != NULL; p = p->next) {
            if (strcmp(p->sv.MALOP, maLop) == 0) {
                coSV = true;
                break;
            }
        }
		if (!coSV) {
            system("cls");
            SetColor(12); cout << " Lop " << maLop << " khong co sinh vien!";
            SetColor(7); cout << "\n Nhan phim bat ky de chon lop khac...";
            getch(); continue;
    }
        system("cls");
        NhanDienConTro(false);
        SetColor(14); 
        cout << "\n\n          ===== BANG THONG KE DIEM TRUNG BINH KHOA HOC =====" << endl;
        cout << "                       Lop: " << maLop << endl << endl;
        SetColor(11);
        cout << "+-----+----------------+---------------------------+----------------+----------+" << endl;
        cout << "| STT |      MASV      |         HO VA TEN         |      LOP       |   DTB    |" << endl;
        cout << "+-----+----------------+---------------------------+----------------+----------+" << endl;
        
		int stt = 1;
        for (PTRSV sv = FirstSV; sv != NULL; sv = sv->next) {
            if (strcmp(sv->sv.MALOP, maLop) != 0) continue;
            float tongDiem_heSo = 0;
            int tongTinChi = 0;
            for (int i = 0; i < ds.n; i++) {
                LopTinChi* ltc = ds.nodes[i];
                for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
                    if (strcmp(p->dk.MASV, sv->sv.MASV) == 0 && !p->dk.huydangky && p->dk.DIEM >= 0) {
                        treeMH mh = TimMH(rootMH, ltc->MAMH);
                        if (mh != NULL) {
                            int soTC = mh->mh.STCLT + mh->mh.STCTH;
                            tongDiem_heSo += p->dk.DIEM * soTC;
                            tongTinChi += soTC;
                        }
                    }
                }
            }
            float dtb = (tongTinChi > 0) ? (tongDiem_heSo / tongTinChi) : 0;
            SetColor(11); cout << "|"; SetColor(15);
            string hoTen = string(sv->sv.HO) + " " + string(sv->sv.TEN);
            
            cout << " " << left << setw(3) << stt++ << " | "
                 << left << setw(14) << sv->sv.MASV << " | "
                 << left << setw(25) << hoTen.substr(0, 25) << " | "
                 << left << setw(14) << sv->sv.MALOP << " | "
                 << right << setw(8) << fixed << setprecision(2) << dtb << " ";
            
            SetColor(11); cout << "|" << endl;
        }
        SetColor(11);
        cout << "+-----+----------------+---------------------------+----------------+----------+" << endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        SetColor(2);
        cout << "\n -> NHAN PHIM BAT KY DE QUAY LAI CHON LOP KHAC...";
        getch();
    }
}

void InBangDiemTongKet(PTRSV FirstSV, DSLTC ds, treeMH rootMH) {
    char maLop[20];
    while (true) {
        ChonLopTongKet(FirstSV, maLop);
        if (strlen(maLop) == 0) return; 
        char dsMaMon[100][11]; int nMon = 0;
        for(int i = 0; i < ds.n; i++) {
            LopTinChi* ltc = ds.nodes[i];
            for (PTRDK p = ltc->dssvdk; p != NULL; p = p->next) {
                PTRSV sv = TimSV(FirstSV, p->dk.MASV);
                if(sv != NULL && strcmp(sv->sv.MALOP, maLop) == 0 && !p->dk.huydangky && p->dk.DIEM >= 0) {
                    bool biTrung = false;
                    for(int k = 0; k < nMon; k++) {
                        if(strcmp(dsMaMon[k], ltc->MAMH) == 0) { biTrung = true; break; }
                    }
                    if(!biTrung && nMon < 100) { strcpy(dsMaMon[nMon++], ltc->MAMH); }
                }
            }
        }

        if (nMon == 0) { 
            system("cls"); 
            SetColor(12); cout << " Lop " << maLop << " khong co du lieu diem!"; 
            SetColor(7); cout << "\n -> Nhan phim bat ky de chon lop khac...";
            getch(); continue; 
        }
        system("cls");
        NhanDienConTro(false);
        SetColor(14); cout << "\n\n                              ===== BANG DIEM TONG KET LOP: " << maLop << " =====" << endl;

        int monMoiBang = 4; 
        for(int batDauMon = 0; batDauMon < nMon; batDauMon += monMoiBang) {
            int ketThucMon = (batDauMon + monMoiBang < nMon) ? batDauMon + monMoiBang : nMon;

            SetColor(11); 
            cout << "\n+-----+----------------+----------------+----------------";
            for (int i = batDauMon; i < ketThucMon; i++) cout << "+------------";
            cout << "+\n| STT |      MASV      |       HO       |      TEN       ";
            for (int i = batDauMon; i < ketThucMon; i++) cout << "|   " << left << setw(9) << dsMaMon[i];
            cout << "|\n+-----+----------------+----------------+----------------";
            for (int i = batDauMon; i < ketThucMon; i++) cout << "+------------";
            cout << "+\n";

            int stt = 1;
            for(PTRSV sv = FirstSV; sv != NULL; sv = sv->next) {
                if(strcmp(sv->sv.MALOP, maLop) != 0) continue;

                SetColor(11); cout << "|"; SetColor(15);
                cout << " " << left << setw(3) << stt++ << " | " << left << setw(14) << sv->sv.MASV << " | "
                     << left << setw(14) << sv->sv.HO << " | " << left << setw(14) << sv->sv.TEN << " ";

                for(int i = batDauMon; i < ketThucMon; i++) {
                    float diemMax = -1;
                    for(int j = 0; j < ds.n; j++) {
                        if(strcmp(ds.nodes[j]->MAMH, dsMaMon[i]) == 0) {
                            for(PTRDK p = ds.nodes[j]->dssvdk; p != NULL; p = p->next) {
                                if(strcmp(p->dk.MASV, sv->sv.MASV) == 0 && !p->dk.huydangky && p->dk.DIEM >= 0) {
                                    if (p->dk.DIEM > diemMax) diemMax = p->dk.DIEM;
                                }
                            }
                        }
                    }
                    SetColor(11); cout << "|"; SetColor(15);
                    if(diemMax >= 0) cout << "    " << left << setw(8) << fixed << setprecision(1) << diemMax;
                    else cout << "            "; 
                }
                SetColor(11); cout << "|\n";
            }
            SetColor(11); cout << "+-----+----------------+----------------+----------------";
            for (int i = batDauMon; i < ketThucMon; i++) cout << "+------------";
            cout << "+\n";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        SetColor(2);
        cout << "\n -> NHAN PHIM BAT KY DE QUAY LAI CHON LOP KHAC...";
        getch(); 
    }
} 

void HuyLTC(DSLTC &dsLTC, treeMH rootMH) {
    int W, H;
    GetConsoleSize(W, H);
    int lX = (W - 85) / 2;
    int lY = 2;
    char locNK[15];
    int hkHienTai;
    LayNienKhoaHocKyHienTai(locNK, hkHienTai);
    char locHK[2];
    locHK[0] = hkHienTai + '0';
    locHK[1] = '\0';
    
    int soDongTrang = 10;
    int trangHienTai = 0; // Biến lưu trang hiện tại
    
    bool reDrawFrame = true;
    bool reLoadData = true;
    
    while (true) {
        if (reDrawFrame) {
            system("cls");
            NhanDienConTro(false);
            SetColor(11);
            gotoxy(lX, lY);
            cout << "========================== HE THONG HUY LOP TIN CHI =============================";
            gotoxy(lX, lY + 1);
            cout << "|                                                                               |";
            gotoxy(lX, lY + 2);
            cout << "|   NIEN KHOA:                  HOC KY:                                         |";
            gotoxy(lX, lY + 3);
            cout << "|                                                                               |";
            gotoxy(lX, lY + 4);
            cout << "=================================================================================";
            gotoxy(lX, lY + 5);
            cout << "| STT | MA LTC | MA MON     | TEN MON HOC                | SV/MIN | TRANG THAI  |";
            gotoxy(lX, lY + 6);
            cout << "|-----|--------|------------|----------------------------|--------|-------------|";
            gotoxy(lX, lY + 17);
            cout << "=================================================================================";
            SetColor(15);
            gotoxy(lX, lY + 19);
            cout << "[<-/->]: Sang trang | [ENTER]: Xac nhan HUY TAT CA lop | [ESC]: Thoat";
            reDrawFrame = false;
        }
        
        int dsIndexHuy[1000];
        int nHuy = 0;
        int hkInt = hkHienTai;
        for (int i = 0; i < dsLTC.n; i++) {
            if (dsLTC.nodes[i] == NULL) continue;
            bool checkNK = strcmp(dsLTC.nodes[i]->NienKhoa, locNK) == 0;
            bool checkHK = dsLTC.nodes[i]->Hocky == hkInt;
            int soSVdk = DemSV_DK(dsLTC.nodes[i]->dssvdk);
            if (checkNK && checkHK && !dsLTC.nodes[i]->huylop && (soSVdk < dsLTC.nodes[i]->sosvmin)) {
                dsIndexHuy[nHuy++] = i;
            }
        }
        
        // Tính toán tổng số trang
        int tongTrang = (nHuy == 0) ? 1 : (nHuy - 1) / soDongTrang + 1;
        if (trangHienTai >= tongTrang) trangHienTai = tongTrang - 1;
        if (trangHienTai < 0) trangHienTai = 0;
        
        if (reLoadData) {
            NhanDienConTro(false);
            SetColor(15);
            gotoxy(lX + 15, lY + 2);
            cout << left << setw(10) << locNK;
            gotoxy(lX + 40, lY + 2);
            cout << left << setw(2) << locHK;
            
            int batDauRow = trangHienTai * soDongTrang; // Vị trí bắt đầu của trang hiện tại
            
            for (int i = 0; i < soDongTrang; i++) {
                gotoxy(lX, lY + 7 + i);
                int idxThuc = batDauRow + i; 
                
                if (idxThuc < nHuy) { // In dữ liệu nếu chưa vượt quá tổng số lớp cần hủy
                    int rIdx = dsIndexHuy[idxThuc];
                    treeMH pMH = TimMH(rootMH, dsLTC.nodes[rIdx]->MAMH);
                    int soSVdk = DemSV_DK(dsLTC.nodes[rIdx]->dssvdk);
                    SetColor(12);
                    cout << "| " << left << setw(3)  << idxThuc + 1 << " | " << left << setw(6) << dsLTC.nodes[rIdx]->MALOPTC
                         << " | " << left << setw(10) << dsLTC.nodes[rIdx]->MAMH << " | " << left << setw(26)
                         << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A") << " | " << right << setw(2)
                         << soSVdk << "/" << left << setw(3) << dsLTC.nodes[rIdx]->sosvmin << " | " << left << setw(11)
                         << "CHO HUY" << " |";
                }
                else { // In dòng trống nếu trang đó không đủ 10 lớp
                    SetColor(11);
                    cout << "|     |        |            |                            |        |             |";
                }
            }
            SetColor(14);
            gotoxy(lX + 15, lY + 18);
            cout << "Trang: " << trangHienTai + 1 << "/" << tongTrang << "   |   Tong so lop du dieu kien huy: " << nHuy << "      ";
            NhanDienConTro(false);
            reLoadData = false;
        }
        
        if (!kbhit()) continue;
        int c = getch();
        
        // Thoát
        if (c == 27) {
            SetColor(15);
            return;
        }
        
        // Cơ chế lật trang bằng mũi tên Trái / Phải
        if (c == 0 || c == 224) {
            c = getch();
            if (c == 75 && trangHienTai > 0) { // Phím mũi tên Trái
                trangHienTai--; 
                reLoadData = true; 
            } 
            else if (c == 77 && trangHienTai < tongTrang - 1) { // Phím mũi tên Phải
                trangHienTai++; 
                reLoadData = true; 
            }
            continue;
        }
        
        // Hủy tất cả
        if (c == 13) {
            if (nHuy == 0) {
                gotoxy(lX, lY + 22);
                SetColor(12);
                cout << "Khong co lop nao thoa man dieu kien de huy!        ";
                Sleep(1500);
                clearLine(lX, lY + 22, 70);
                reLoadData = true;
            }
            else {
                gotoxy(lX, lY + 22);
                SetColor(13);
                cout << "BAN CO CHAC CHAN MUON HUY " << nHuy << " LOP TREN KHONG? (Y/N): _";
                gotoxy(wherex() - 1, wherey());
                char confirm = getch();
                cout << confirm;
                if (toupper(confirm) == 'Y') {
                    for (int i = 0; i < nHuy; i++) {
                        dsLTC.nodes[dsIndexHuy[i]]->huylop = true;
                    }
                    gotoxy(lX, lY + 23);
                    SetColor(10);
                    cout << "THANH CONG! Cac lop da chuyen sang trang thai HUY.";
                    Sleep(2000);
                    trangHienTai = 0; // Trở về trang đầu sau khi hủy xong
                    reDrawFrame = true;
                    reLoadData = true;
                }
                else {
                    gotoxy(lX, lY + 23);
                    SetColor(14);
                    cout << "DA HUY THAO TAC.";
                    Sleep(1500);
                    clearLine(lX, lY + 22, 70);
                    clearLine(lX, lY + 23, 70);
                    reLoadData = true;
                }
            }
        }
    }
}

void LayGoiY_LTC(DSLTC &ds, char *input, int dsIndex[], int &n) 
{
    n = 0;
    for (int i = 0; i < ds.n; i++) 
	{
        char maStr[15]; sprintf(maStr, "%d", ds.nodes[i]->MALOPTC);
        if (strlen(input) == 0 || strstr(maStr, input) != NULL) 
		{
            dsIndex[n++] = i;
        }
    }
}

void VeKhungGoiY_LTC(DSLTC &ds, int dsIndex[], int nGoiY, int pointer, int trangHienTai, int startX, int startY) {
    int boardX = startX; 
    int boardY = startY + 5; 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetColor(11);
    gotoxy(boardX, boardY);     cout << "========================================================";
    gotoxy(boardX, boardY + 1); cout << "| MA LTC |   MA MON   | NHOM | HK |      NIEN KHOA     |";
    gotoxy(boardX, boardY + 2); cout << "|--------|------------|------|----|--------------------|";
	int batDau = trangHienTai * 8; 
    for (int i = 0; i < 8; i++) 
	{ 
        int idx = batDau + i; 
        gotoxy(boardX, boardY + 3 + i);
        if (idx < nGoiY) 
		{
            int rIdx = dsIndex[idx];
            if (idx == pointer) 
			{
                SetConsoleTextAttribute(hConsole, 79); 
                cout << "| " << left << setw(6) << ds.nodes[rIdx]->MALOPTC 
                     << " | " << left << setw(10) << ds.nodes[rIdx]->MAMH 
                     << " | " << left << setw(4) << ds.nodes[rIdx]->Nhom 
                     << " | " << left << setw(2) << ds.nodes[rIdx]->Hocky 
                     << " | " << left << setw(18) << ds.nodes[rIdx]->NienKhoa << " |";
                SetConsoleTextAttribute(hConsole, 7);
            } 
			else 
			{
                SetColor(11); cout << "| "; SetColor(15); cout << left << setw(6) << ds.nodes[rIdx]->MALOPTC;
                SetColor(11); cout << " | "; SetColor(15); cout << left << setw(10) << ds.nodes[rIdx]->MAMH;
                SetColor(11); cout << " | "; SetColor(15); cout << left << setw(4) << ds.nodes[rIdx]->Nhom;
                SetColor(11); cout << " | "; SetColor(15); cout << left << setw(2) << ds.nodes[rIdx]->Hocky;
                SetColor(11); cout << " | "; SetColor(15); cout << left << setw(18) << ds.nodes[rIdx]->NienKhoa;
                SetColor(11); cout << " |";
            }
        } 
		else 
		{
            SetColor(11); cout << "|        |            |      |    |                    |"; 
        }
    }
    gotoxy(boardX, boardY + 11); cout << "========================================================";
	SetColor(14);     gotoxy(boardX + 20, boardY + 12);
    int tongTrang = (nGoiY > 0) ? (nGoiY - 1) / 8 + 1 : 1;
    cout << " Trang: " << trangHienTai + 1 << "/" << tongTrang ;
    SetColor(15);
}
int DemSoGoiY_LTC(DSLTC &ds, char* input) {
    int count = 0;
    for (int i = 0; i < ds.n; i++) {
        char maLopStr[15];
        itoa(ds.nodes[i]->MALOPTC, maLopStr, 10); 
        
        if (strlen(input) == 0 || strstr(maLopStr, input) != NULL) {
            count++;
        }
    }
    return count;
}

bool XoaLTC(DSLTC &ds) {
    char input[15] = ""; int nIn = 0;
    int screenW = 0, screenH = 0; GetConsoleSize(screenW, screenH);
    int lX = (screenW - 60) / 2, lY = (screenH - 28) / 2;
	bool reDrawFrame = true; 
    bool reLoadData = true;  
    int ptrLTC = 0; 

	while (true) 
	{	
		int nGoiY = DemSoGoiY_LTC(ds, input);
        int sSize = (nGoiY == 0) ? 1 : nGoiY;
        int* dsGoiY = new int[sSize];
        if (reDrawFrame) 
		{
            system("cls");
            SetColor(11);
            gotoxy(lX, lY + 0); cout << "====================XOA LOP TIN CHI ====================";
            gotoxy(lX, lY + 1); cout << "|                                                      |";
            gotoxy(lX, lY + 2); cout << "|   NHAP MA LTC CAN XOA:                               |";
            gotoxy(lX, lY + 3); cout << "|                                                      |";
            gotoxy(lX, lY + 4); cout << "========================================================";
            SetColor(15);
            gotoxy(lX, lY + 19); cout <<"[UP/DOWN]: Di chuyen      [ENTER]: Xoa      [ESC]: Thoat"; 
            reDrawFrame = false;
        }
        if (reLoadData) 
		{
            NhanDienConTro(false);
			int nGoiYActual = 0;
        	LayGoiY_LTC(ds, input, dsGoiY, nGoiYActual);
            if (ptrLTC >= nGoiY && nGoiY > 0) ptrLTC = nGoiY - 1;
            int trang = (nGoiY == 0) ? 0 : ptrLTC / 8;
            SetColor(15); 
            gotoxy(lX + 25, lY + 2); cout << "               "; 
            gotoxy(lX + 25, lY + 2); cout << input;
            VeKhungGoiY_LTC(ds, dsGoiY, nGoiY, ptrLTC, trang, lX, lY);
            NhanDienConTro(true); 
            gotoxy(lX + 25 + nIn, lY + 2);
            reLoadData = false;
        }
		if (!kbhit()) continue; 
		int c = getch();
        if (c == 27)
		{	
			delete[] dsGoiY;
        	SetColor(15);
        	return false;
		} 
        if (c == 0 || (unsigned char)c == 224) 
		{
            c = getch();
            if (c == 72 && ptrLTC > 0) { ptrLTC--; reLoadData = true; }
            else if (c == 80 && ptrLTC < nGoiY - 1) { ptrLTC++; reLoadData = true; }
            else if (c == 77) { ptrLTC += 8; if (ptrLTC >= nGoiY) ptrLTC = nGoiY - 1; reLoadData = true; }
            else if (c == 75) { ptrLTC -= 8; if (ptrLTC < 0) ptrLTC = 0; reLoadData = true; }
        }
        else if (c == 13) 
		{ 
            if (nGoiY > 0)
			{
                int idxChon = dsGoiY[ptrLTC];
                if (ds.nodes[idxChon]->dssvdk != NULL) 
				{
                    SetColor(12);
                    gotoxy(lX, lY + 22); cout << " LOI: Lop da co SV dang ky! Khong the xoa.           ";
        			Sleep(2000);
                    gotoxy(lX, lY + 22); cout << "                                                        ";
                    reLoadData = true;
                } 
				else 
				{
                    int boxX = lX + 5, boxY = lY + 22;
					SetColor(13);
					gotoxy(boxX, boxY + 0); cout << "_________________________________________";
					gotoxy(boxX, boxY + 1); cout << "|   BAN CO CHAC CHAN MUON XOA KHONG?    |";
					gotoxy(boxX, boxY + 2); cout << "|                                       |";
					gotoxy(boxX, boxY + 3); cout << "|   Nhap lua chon cua ban (Y/N): _      |";
					gotoxy(boxX, boxY + 4); cout << "|_______________________________________|";
					gotoxy(boxX + 33, boxY + 3);
					char xacNhan = getch(); 
					cout << xacNhan; 
					if (toupper(xacNhan) == 'Y') 
					{
					    delete ds.nodes[idxChon];
					    for (int i = idxChon; i < ds.n - 1; i++) ds.nodes[i] = ds.nodes[i + 1];
					    ds.n--;
						SetColor(10);
					    gotoxy(boxX + 5, boxY + 6); cout << "SUCCESS: DA XOA THANH CONG!";
					    Sleep(2000); 
					    input[0] = '\0';
					    nIn = 0; 
					    ptrLTC = 0;
						reDrawFrame = true; 
					    reLoadData = true;
					}
					else 
					{
					    SetColor(14);
					    gotoxy(boxX + 5, boxY + 6); cout << "DA HUY THAO TAC.";
					    Sleep(1500); 
					    reDrawFrame = true; 
					    reLoadData = true;
					}
				}
            }
        }
        else if (c == 8 && nIn > 0) 
		{ 
            input[--nIn] = '\0'; ptrLTC = 0; reLoadData = true; 
        }
        else if (isdigit(c) && nIn < 10) 
		{ 
            input[nIn++] = c; input[nIn] = '\0'; ptrLTC = 0; reLoadData = true; 
        }
    delete[] dsGoiY;
    } 
    return true;
}

void InDanhSachDaDangKy(DSLTC &dsLTC, treeMH rootMH, int dsIndexDK[], int lX, int lY, int trangDK, int indexHuy, bool dangHuy, int nDK, int soDongTrangDK) 
{
    SetColor(11); 
    gotoxy(lX, lY + 19); cout << "==================== CAC MON DA DANG KY =====================";
    gotoxy(lX, lY + 20); cout << "| STT | MA LTC | MA MON | TEN MON HOC                | NHOM |";
    gotoxy(lX, lY + 21); cout << "|-----|--------|--------|----------------------------|------|";

	int tongTrangDK = (nDK + soDongTrangDK - 1) / soDongTrangDK;
    if (tongTrangDK == 0) tongTrangDK = 1;
    int batDau = (trangDK - 1) * soDongTrangDK;
	for (int i = 0; i < soDongTrangDK; i++)
	{
        int thuc = batDau + i;
        gotoxy(lX, lY + 22 + i);
        if (thuc < nDK) 
		{
            int rIdx = dsIndexDK[thuc];
            treeMH pMH = TimMH(rootMH, dsLTC.nodes[rIdx]->MAMH);
            if (dangHuy && thuc == indexHuy) 
   	 			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79); 
			else 
    			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); 
			SetColor(11); cout << "| "; 
			SetColor(15); cout << left << setw(3) << thuc + 1;
			SetColor(11); cout << " | ";
			SetColor(15); cout << left << setw(6) << dsLTC.nodes[rIdx]->MALOPTC;
			SetColor(11); cout << " | ";
			SetColor(15); cout << left << setw(6) << dsLTC.nodes[rIdx]->MAMH;
			SetColor(11); cout << " | ";
			SetColor(15); cout << left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A");
			SetColor(11); cout << " | ";
			SetColor(15); cout << left << setw(4) << dsLTC.nodes[rIdx]->Nhom;
			SetColor(11); cout << " |";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        } 
		else 
		{
            SetColor(11); cout << "|     |        |        |                            |      |";
        }
    }
    SetColor(14); gotoxy(lX + 45, lY + 31); 
    cout << " Trang DK: " << trangDK << "/" << tongTrangDK ;
    gotoxy(lX, lY + 29); SetColor(11); cout << "=============================================================";
}

int DangKyLTC(DSLTC &dsLTC, PTRSV &FirstSV, treeMH rootMH) 
{
    char maSV_Chon[16] = "";
    int W, H;
    GetConsoleSize(W, H);
    int khungW = 52; 
    int khungH = 4;  
    int lX = (W - 105) / 2; 
    int lY = (H - 30) / 2;  

    while (true) 
    {
        system("cls");
        SetColor(11);
        gotoxy(lX + 25, lY + 2); cout << "+" << string(50, '-') << "+";
        gotoxy(lX + 25, lY + 3); cout << "|        DANG KY LOP TIN CHI - NHAP MA SV          |";
        gotoxy(lX + 25, lY + 4); cout << "|  Ma SV:                                          |";
        gotoxy(lX + 25, lY + 5); cout << "+" << string(50, '-') << "+";
        SetColor(14); 
        gotoxy(lX + 25, lY + 7); cout << "ENTER: Xac nhan                           ESC: Thoat";
        SetColor(15); gotoxy(lX + 35, lY + 4); AnConTro(false);
        char* chuoiNhap = nhap_ma(); 
        fflush(stdin);
        if (chuoiNhap == NULL || strlen(chuoiNhap) == 0) return 0; 
        strcpy(maSV_Chon, chuoiNhap);

        PTRSV pSV = TimSV(FirstSV, maSV_Chon);
        if (pSV != NULL) 
        {
            system("cls");
            SetColor(11);
            gotoxy(lX + 25, lY + 2); cout << "+" << string(55, '-') << "+";
            gotoxy(lX + 25, lY + 3); cout << "|               THONG TIN SINH VIEN DANG KY             |";
            gotoxy(lX + 25, lY + 4); cout << "|-------------------------------------------------------|";
            SetColor(15);
            gotoxy(lX + 25, lY + 5); SetColor(11); cout << "|"; SetColor(15); cout << " Ma so SV  : " << left << setw(37) << pSV->sv.MASV; SetColor(11); cout << "     |";
            gotoxy(lX + 25, lY + 6); SetColor(11); cout << "|"; SetColor(15); cout << " Ho va ten : " << left << setw(37) << (string(pSV->sv.HO) + " " + pSV->sv.TEN); SetColor(11); cout << "     |";
            gotoxy(lX + 25, lY + 7); SetColor(11); cout << "|"; SetColor(15); cout << " Gioi tinh : " << left << setw(37) << pSV->sv.PHAI; SetColor(11); cout << "     |";
            gotoxy(lX + 25, lY + 8); SetColor(11); cout << "|"; SetColor(15); cout << " So DT     : " << left << setw(37) << pSV->sv.SODT; SetColor(11); cout << "     |";
            gotoxy(lX + 25, lY + 9); SetColor(11); cout << "|"; SetColor(15); cout << " Ma lop    : " << left << setw(37) << pSV->sv.MALOP; SetColor(11); cout << "     |";
            gotoxy(lX + 25, lY + 10); cout << "+" << string(55, '-') << "+";
            SetColor(14);
            gotoxy(lX + 25, lY + 12); cout << "ENTER: Dong y                         ESC: Nhap lai ma SV";
            AnConTro(true);
            int phimBam = getch(); 
            if (phimBam == 13) break;    
            if (phimBam == 27) continue;
        } 
        else 
        {
            SetColor(12); gotoxy(lX + 25, lY + 8); 
            cout << "Loi: Khong tim thay Ma SV " << maSV_Chon << "!";
            Sleep(1500);
        }
    }

    char nienKhoaCoi[15];
    int hocKyCoi;
    LayNienKhoaHocKyHienTai(nienKhoaCoi, hocKyCoi);  
    int indexThanhSang = 0; 
    int trangDK = 1;  
    int indexHuy = 0;
    int soDongTrang = 10;
    int soDongTrangDK = 7;
    int focusMode = 0;
    int nDK = 0; 
    int* dsIndexDK = new int[dsLTC.n];
    bool reDrawFrame = true; 
    bool reLoadData = true; 

    while (true) 
    {
        if (reDrawFrame) 
        {
            system("cls");
            SetColor(11);
            gotoxy(lX, lY);     cout << "===================================================================================";
            gotoxy(lX, lY + 1); cout << "|                                                                                 |";
            gotoxy(lX, lY + 2); cout << "|   NIEN KHOA:                HOC KY:                                             |";
            gotoxy(lX, lY + 3); cout << "|                                                                                 |";
            gotoxy(lX, lY + 4); cout << "===================================================================================";
            gotoxy(lX, lY + 5); cout << "| STT | MA LTC | MA MON | TEN MON HOC                | NHOM | SO SVDK | CON TRONG |";
            gotoxy(lX, lY + 6); cout << "|-----|--------|--------|----------------------------|------|---------|-----------|";
            gotoxy(lX, lY + 17);cout << "===================================================================================";
            SetColor(15); 
            gotoxy(lX+90, lY + 0);cout << "[UP/DOWN]: Di chuyen  ";
            gotoxy(lX+90, lY + 2);cout << "[<-/->]  : Sang trang ";  
            gotoxy(lX+90, lY + 1);cout << "[ENTER]  : Hoan tat ";
            gotoxy(lX+90, lY + 3);cout << "[ESC]    : Thoat."; 
            SetColor(14); 
            gotoxy(lX+90, lY + 5);cout << "[F5]: HUY MON & DANG KI";      
            reDrawFrame = false;
        }

        SetColor(15);
        gotoxy(lX + 15, lY + 2); cout << left << setw(15) << nienKhoaCoi;
        gotoxy(lX + 38, lY + 2); cout << left << setw(5) << hocKyCoi;
        int* dsIndexLoc = new int[dsLTC.n]; int nLoc = 0;
        for (int i = 0; i < dsLTC.n; i++) 
        {
            bool checkNK = strstr(dsLTC.nodes[i]->NienKhoa, nienKhoaCoi) != NULL;
            bool checkHK = dsLTC.nodes[i]->Hocky == hocKyCoi;
            if (checkNK && checkHK && !dsLTC.nodes[i]->huylop) dsIndexLoc[nLoc++] = i;
        }

        int tongTrang = (nLoc + soDongTrang - 1) / soDongTrang; if (tongTrang == 0) tongTrang = 1;
        int trangHienTai = (indexThanhSang / soDongTrang) + 1;
        int batDauRow = (trangHienTai - 1) * soDongTrang;

        if (reLoadData)
        {	
			nDK=0;
            NhanDienConTro(false);
   			for (int i = 0; i < dsLTC.n; i++)
			{
       			PTRDK p = dsLTC.nodes[i]->dssvdk;
        		while (p != NULL) 
				{
            		if (strcmp(p->dk.MASV, maSV_Chon) == 0 && p->dk.huydangky == false) 
					{
                		dsIndexDK[nDK++] = i;
                		break;
            		}
            		p = p->next;
        		}
   			}
            
            for (int i = 0; i < soDongTrang; i++) 
            {
                int thuc = batDauRow + i;
                gotoxy(lX, lY + 7 + i);
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        		if (thuc < nLoc) 
                {
                    int rIdx = dsIndexLoc[thuc];
                    treeMH pMH = TimMH(rootMH, dsLTC.nodes[rIdx]->MAMH);
                    int soSVdk = DemSV_DK(dsLTC.nodes[rIdx]->dssvdk);
                    int conTrong = dsLTC.nodes[rIdx]->sosvmax - soSVdk;
        
                    if (focusMode == 0 && thuc == indexThanhSang) SetConsoleTextAttribute(hConsole, 79);
                    else SetConsoleTextAttribute(hConsole, 15);

                    SetColor(11);cout << "| ";  SetColor(15);cout<< left << setw(3)  << thuc + 1;
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(6)  << dsLTC.nodes[rIdx]->MALOPTC;
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(6)  << dsLTC.nodes[rIdx]->MAMH;
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(26) << (pMH ? string(pMH->mh.TENMH).substr(0, 26) : "N/A"); 
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(4)  << dsLTC.nodes[rIdx]->Nhom ;
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(7)  << soSVdk;
                    SetColor(11);cout<< " | " ;SetColor(15);cout<< left << setw(9)  << conTrong; SetColor(11);cout<< " |";
            
                    SetConsoleTextAttribute(hConsole, 15);
                } 
                else 
                {
                    SetColor(11); 
                    cout << "|     |        |        |                            |      |         |           |";
                }
            }
            
            SetColor(14); 
            gotoxy(lX + 70, lY + 18); 
            cout << " Trang: " << trangHienTai << "/" << tongTrang;
          	InDanhSachDaDangKy(dsLTC, rootMH, dsIndexDK, lX+10, lY, trangDK, indexHuy, (focusMode == 1), nDK, soDongTrangDK);
            reLoadData = false;
        }
		int c = getch();
		if (c == 27) 
		{  	NhanDienConTro(true);
			delete[] dsIndexDK;
    		delete[] dsIndexLoc;
			SetColor(15);
			return 0; 
		}
		if (c == 0 || c == 224) 
		{
		    int maPhu = getch(); 
		    
		    if (maPhu == 63) 
		    {
		        NhanDienConTro(false);
		        reLoadData = true;
		        
		        if (focusMode == 0) 
		        {
		            if (nDK > 0) 
		            {
		                focusMode = 1;
		                indexHuy = 0; 
		                trangDK = 1;
		            }
		            else 
		            {
		                SetColor(12); gotoxy(lX + 80, lY + 25);
		                cout << "Chua co mon de huy!";
		                Sleep(1000); gotoxy(lX + 80, lY + 25); cout << string(20, ' ');
		            }
		        }
		        else 
		        {
		            focusMode = 0; 
		        }
		    }
		    else if (focusMode == 0) 
		    {
		        NhanDienConTro(false); 
		        reLoadData = true;
		
		        if (maPhu == 72) 
		        {
		            if (indexThanhSang == 0) 
		            {
		                indexThanhSang = nLoc - 1; 
		            }
		            else 
		            {
		                indexThanhSang--; 
		            }
		        }
		        else if (maPhu == 80) 
		        {
		            if (indexThanhSang == nLoc - 1) 
		            {
		                indexThanhSang = 0; 
		            }
		            else 
		            {
		                indexThanhSang++; 
		            }
		        }
		        else if (maPhu == 77) 
		        {
		            indexThanhSang = (indexThanhSang + soDongTrang < nLoc) ? (indexThanhSang + soDongTrang) : (nLoc - 1);
		        }
		        else if (maPhu == 75) 
		        {
		            indexThanhSang = (indexThanhSang - soDongTrang >= 0) ? (indexThanhSang - soDongTrang) : 0;
		        }
		    } 
		    else if (focusMode == 1) 
		    {
		        NhanDienConTro(false); 
		        reLoadData = true;
		        int soDongTrangDK = 7; 
		
		        if (maPhu == 72) 
		        {
		            if (indexHuy == 0) 
		            {
		                indexHuy = nDK - 1; 
		            }
		            else 
		            {
		                indexHuy--;
		            }
		            trangDK = (indexHuy / soDongTrangDK) + 1; 
		        }
		        else if (maPhu == 80) 
		        { 
		            if (indexHuy == nDK - 1) 
		            {
		                indexHuy = 0; 
		            }
		            else
		            {
		                indexHuy++;
		            }
		            trangDK = (indexHuy / soDongTrangDK) + 1; 
		        }
		        else if (maPhu == 77) 
		        {
		            indexHuy = (indexHuy + soDongTrangDK < nDK) ? (indexHuy + soDongTrangDK) : (nDK - 1);
		            trangDK = (indexHuy / soDongTrangDK) + 1; 
		        }
		        else if (maPhu == 75) 
		        {
		            indexHuy = (indexHuy - soDongTrangDK >= 0) ? (indexHuy - soDongTrangDK) : 0;
		            trangDK = (indexHuy / soDongTrangDK) + 1; 
		        }
		    }
		}
		        else if (c == 13) 
        { 
            if (focusMode == 0) 
            { 
                if (nLoc > 0) 
                {
                    int i = dsIndexLoc[indexThanhSang];
                    int soSVdk = DemSV_DK(dsLTC.nodes[i]->dssvdk);
                    bool checkLoi = false;
                
                    if (soSVdk >= dsLTC.nodes[i]->sosvmax) 
                    {
                        SetColor(12); gotoxy(lX + 80, lY + 25); 
                        cout << "Loi: Lop da day!                                 "; 
                        checkLoi = true;
                    }
                    else 
                    {
                        PTRDK p = dsLTC.nodes[i]->dssvdk;
                        while (p != NULL) 
                        {
                            if (strcmp(p->dk.MASV, maSV_Chon) == 0 && p->dk.huydangky == false) 
                            {
                                SetColor(12); gotoxy(lX + 80, lY + 25);
                                if (p->dk.DIEM >= 4) cout << "SV da hoan thanh mon hoc nay!    ";
                                else cout << "SV da dang ky lop nay!       ";
                                checkLoi = true; break;
                            }
                            p = p->next;
                        }
                    }
					
					if (!checkLoi) 
                    {
                        for (int j = 0; j < dsLTC.n; j++) 
                        {
                            if (strcmp(dsLTC.nodes[j]->MAMH, dsLTC.nodes[i]->MAMH) == 0) 
                            {
                                PTRDK q = dsLTC.nodes[j]->dssvdk;
                                while (q != NULL) 
                                {
                                    if (strcmp(q->dk.MASV, maSV_Chon) == 0 && q->dk.huydangky == false)
                                    {
                                        SetColor(12); gotoxy(lX + 78, lY + 25);
                                        cout << "SV da dang ky 1 nhom cua mon nay!";
                                        checkLoi = true; break;
                                    }
                                    q = q->next;
                                }
                            }
                            if (checkLoi) break;
                        }
                    }

                    if (checkLoi) 
                    {
                        NhanDienConTro(false);
                        Sleep(1500);
                        while(kbhit()) getch(); 
                        gotoxy(lX + 78, lY + 25); cout << string(35, ' '); 
                        continue; 
                    }

                    PTRDK newDK = new nodeDK;
                    strcpy(newDK->dk.MASV, maSV_Chon);
                    newDK->dk.DIEM = 0;
                    newDK->dk.huydangky = false;
                    newDK->next = dsLTC.nodes[i]->dssvdk;
                    dsLTC.nodes[i]->dssvdk = newDK;
                    
                    SetColor(10); gotoxy(lX + 80, lY + 25); 
                    cout << "Dang ky thanh cong !";
                    SaveFileDSLTC(dsLTC, filenameDSLTC);
                    Sleep(1500);
                    while(kbhit()) getch();
                    gotoxy(lX + 80, lY + 25); cout << string(25, ' ');
                    reLoadData = true;
                }
            }
            else 
            { 
                if (nDK>0)
                {
					int i = dsIndexDK[indexHuy];
                    PTRDK p = dsLTC.nodes[i]->dssvdk;
                    while (p != NULL) 
                    {
                        if (strcmp(p->dk.MASV, maSV_Chon) == 0 && !p->dk.huydangky) 
                        {
                            p->dk.huydangky = true;
                            break;
                    	}
                    	p=p->next;
            		}
								 
                    SetColor(10); gotoxy(lX + 80, lY + 25);
                    cout << "Huy mon thanh cong !";
                    SaveFileDSLTC(dsLTC, filenameDSLTC);
                    Sleep(1000); 
                    gotoxy(lX + 80, lY + 25); cout << string(25, ' ');
                    
                    if (nDK > 1) 
                    {
                        if (indexHuy >= nDK - 1) indexHuy = nDK - 2; 
                        int tongTrangDK_Moi = (nDK - 1 + 6) / 7;
                        if (trangDK > tongTrangDK_Moi) trangDK = tongTrangDK_Moi;
                    }
                    else 
                    {
                        indexHuy = 0;
                        focusMode = 0; 
                    }
                    reLoadData = true;
                }
            }
        }
        delete[] dsIndexLoc;
    }
    delete[] dsIndexDK;
    return 1;
}

bool HieuChinhLTC(DSLTC &ds, treeMH &rootMH) 
{
    if (ds.n == 0) return false;
    int sX = 30, sY = 5; 
    char search[15] = ""; 
    int nIn = 0;     
    int ptrLTC = 0;    
    int tMH = 1;     
    char nienKhoaHeThong[15];
    int hocKyHeThong;
    LayNienKhoaHocKyHienTai(nienKhoaHeThong, hocKyHeThong);
    int namHienTaiHeThong = atoi(nienKhoaHeThong);
	while (true) 
    {	int nGoiY = DemSoGoiY_LTC(ds, search);
    	int sSize = (nGoiY == 0) ? 1 : nGoiY;
    	int* dsGoiY = new int[sSize];
        system("cls");
        AnConTro(true);
        SetColor(11);
        gotoxy(sX, sY);     cout << "========================================================";
        gotoxy(sX, sY + 1); cout << "|                                                      |";
        gotoxy(sX, sY + 2); cout << "|   NHAP MA LTC CAN SUA:                               |";
        gotoxy(sX, sY + 3); cout << "|                                                      |";
        gotoxy(sX, sY + 4); cout << "========================================================";
        SetColor(7);
        gotoxy(sX, sY + 20); cout << "[Up/Down]: Di chuyen      [Enter]: Sua     [ESC]: Thoat ";
        while (true) 
        {
            AnConTro(true);
			int nGoiYActual = 0; 
            LayGoiY_LTC(ds, search, dsGoiY, nGoiYActual);
            int trangLTC = (nGoiY == 0) ? 0 : ptrLTC / 8;
            SetColor(15);
            gotoxy(sX + 25, sY + 2); cout << setw(15) << left << search;
            VeKhungGoiY_LTC(ds, dsGoiY, nGoiY, ptrLTC, trangLTC, sX, sY);
            AnConTro(false); gotoxy(sX + 25 + nIn, sY + 2); 
            int c = getch();
            if (c == 27) 
            {
                delete[] dsGoiY;
                return false;
            }
           if (c == 0 || (unsigned char)c == 224) 
            {
                c = getch();
                if (c == 72 && ptrLTC > 0) ptrLTC--; 
                else if (c == 80 && ptrLTC < nGoiY - 1) ptrLTC++; 
                else if (c == 77) { 
                    ptrLTC += 8; 
                    if (ptrLTC >= nGoiY) ptrLTC = nGoiY - 1; 
                } 
                else if (c == 75) { 
                    ptrLTC -= 8; 
                    if (ptrLTC < 0) ptrLTC = 0; 
                } 
            }
            else if (c == 13 && nGoiY > 0) break; 
            else if (c == 8 && nIn > 0) { search[--nIn] = '\0'; ptrLTC = 0; } 
            else if (isdigit(c) && nIn < 10) 
            { 
                search[nIn++] = c; 
                search[nIn] = '\0'; 
                ptrLTC = 0; 
            }
        }
        
        int viTri = dsGoiY[ptrLTC];
        LopTinChi &ltcG = *(ds.nodes[viTri]); 
        LopTinChi ltcT = ltcG;              
        int col = 0; 
        bool backToSearch = false;
        bool veLaiKhungChinh = true; 
        system("cls");
        int bX = 63, bY = 4; 
        
        while (true) 
		{
            if (veLaiKhungChinh)
            {
                system("cls");
                AnConTro(true);
                SetColor(11); 
                gotoxy(bX, bY);     cout << "=========================================================";
                gotoxy(bX, bY + 1); cout << "|                 HIEU CHINH LOP TIN CHI                |";
                gotoxy(bX, bY + 2); cout << "=========================================================";
                for(int i = 3; i <= 12; i++) { gotoxy(bX, bY + i); cout << "|                                                       |"; }
                gotoxy(bX, bY + 13); cout <<"=========================================================";
                
                SetColor(15);
                gotoxy(bX + 4, bY + 5);  cout << "0. Nien khoa       :"; 
                gotoxy(bX + 4, bY + 6);  cout << "1. Hoc ky          :";
                gotoxy(bX + 4, bY + 7);  cout << "2. Ma mon hoc      :";
                gotoxy(bX + 4, bY + 8);  cout << "3. Nhom            :";
                gotoxy(bX + 4, bY + 9);  cout << "4. So SV Toi thieu :";
                gotoxy(bX + 4, bY + 10); cout << "5. So SV Toi da    :"; 
                SetColor(7);
                gotoxy(bX, bY + 15); cout << "[Up/Down]: Di chuyen      [Enter]: Luu      [ESC]: Thoat"; 
                veLaiKhungChinh = false;
            }
			AnConTro(true);
            VeTraCuuMonHoc(rootMH, tMH); 
            SetColor(14); gotoxy(bX + 4, bY + 3); cout << "DANG SUA LTC SO: " << ltcT.MALOPTC;
            for (int i = 0; i < 6; i++) 
            {
                int dongIn = i + 5; 
                gotoxy(bX + 26, bY + dongIn);
                
                if (col == i) SetColor(143); else SetColor(15);
                switch (i) {
                    case 0: cout << " " << setw(27) << left << ltcT.NienKhoa; break;
                    case 1: cout << " " << setw(27) << left << ltcT.Hocky; break;
                    case 2: cout << " " << setw(27) << left << ltcT.MAMH; break;
                    case 3: cout << " " << setw(27) << left << ltcT.Nhom; break;
                    case 4: cout << " " << setw(27) << left << ltcT.sosvmin; break;
                    case 5: cout << " " << setw(27) << left << ltcT.sosvmax; break;
                }
            }
            AnConTro(false); SetColor(15);
            int cur = 0;
            switch (col) {
            	
                case 0: cur = strlen(ltcT.NienKhoa); break;
                case 1: cur = (ltcT.Hocky < 10 ? 1 : 2); break;
                case 2: cur = strlen(ltcT.MAMH); break;
                case 3: cur = (ltcT.Nhom < 10 ? 1 : (ltcT.Nhom < 100 ? 2 : 3)); break;
                case 4: cur = (ltcT.sosvmin < 10 ? 1 : (ltcT.sosvmin < 100 ? 2 : 3)); break;
                case 5: cur = (ltcT.sosvmax < 10 ? 1 : (ltcT.sosvmax < 100 ? 2 : 3)); break;
            }
            gotoxy(bX + 27 + cur, bY + 5 + col);
            
            int ch = getch();
            if (ch == 0 || (unsigned char)ch == 224) 
            {
                ch = getch();
                if (ch == 72) col = (col > 0) ? col - 1 : 5; 
                if (ch == 80) col = (col < 5) ? col + 1 : 0; 
                if (ch == 75 && tMH > 1) tMH--;
                if (ch == 77) tMH++;
                continue;
            }
            if (ch == 27) { backToSearch = true; break; } 
            if (ch == 13) 
            {   
                int namDau, namCuoi;
                if (sscanf(ltcT.NienKhoa, "%d-%d", &namDau, &namCuoi) != 2 || namCuoi != namDau + 1)
                {
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: Nien khoa phai dang YYYY-YYYY! ";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 0; continue;
                }

                if (ltcT.Hocky != 1 && ltcT.Hocky != 2)
                {
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: Hoc ky chi duoc phep la 1 hoac 2!";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 1; continue;
                }

                if (LaQuaKhu(ltcT.NienKhoa, ltcT.Hocky))
                {
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: Khong duoc sua thoi gian ve qua khu! ";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 0; continue;
                }

                bool biTrung = false;
                for (int i = 0; i < ds.n; i++) 
                {
                    if (i != viTri && strcmp(ltcT.MAMH, ds.nodes[i]->MAMH) == 0 &&
                        strcmp(ltcT.NienKhoa, ds.nodes[i]->NienKhoa) == 0 &&
                        ltcT.Hocky == ds.nodes[i]->Hocky && ltcT.Nhom == ds.nodes[i]->Nhom)
                    {
                        biTrung = true; break;
                    }
                }
                if (biTrung) {
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: LTC BI TRUNG THONG TIN !        ";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 2; continue;
                } 
                
                int soLuongHienTai = DemSV_DK(ltcT.dssvdk);
                if (ltcT.sosvmin > soLuongHienTai){
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: SV MIN PHAI <= SV DANG KI !";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 4; continue;
                }
                if (ltcT.sosvmax < soLuongHienTai){
                    SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: SV MAX PHAI >= SV DANG KI !";
                    Sleep(2000); clearLine(bX + 10, bY + 14, 45); col = 5; continue;
                }
                
                ltcG = ltcT; 
                SetColor(10); gotoxy(bX + 10, bY + 14); cout << "SUCCESS: DA LUU THANH CONG!";
                Sleep(1500); backToSearch = true; break;
            }
            
            ungetch(ch);
            SetColor(15);
            
            if (col == 0) 
            {
                clearLine(bX + 27, bY + 5, 26);
                char *s = nhap_ma();
                if (strlen(s) > 0) {
                    strcpy(ltcT.NienKhoa, s);
                    col = 1; 
                }
                delete[] s; fflush(stdin);
            }
            else if (col == 1)
            {
                clearLine(bX + 27, bY + 6, 26);
                char *s = nhap_so();
                if (strlen(s) > 0) {
                    ltcT.Hocky = atoi(s);
                    col = 2; 
                }
                delete[] s; fflush(stdin);
            }
            else if (col == 2) 
            { 
                clearLine(bX + 27, bY + 7, 26);
                char *s = nhap_ma();
                if (strlen(s) > 0) {
                    if (TimMH(rootMH, s) == NULL) { 
                        SetColor(12); gotoxy(bX + 10, bY + 14); cout << "LOI: MON HOC KHONG TON TAI !     ";
                        Sleep(1500); clearLine(bX + 10, bY + 14, 45);
                    } else {
                        strcpy(ltcT.MAMH, s);
                        col = 3;
                    }
                }
                delete[] s; fflush(stdin);
            } 
            else if (col == 3) 
            { 
                clearLine(bX + 27, bY + 8, 26);
                char *s = nhap_so();
                if (strlen(s) > 0) {
                    ltcT.Nhom = atoi(s);
                    col = 4;
                }
                delete[] s; fflush(stdin);
            }
            else
            { 
                int dongNhap = bY + col + 5;
                clearLine(bX + 27, dongNhap, 26);
                char *v = nhap_so();
                if (strlen(v) > 0) 
                {
                    int val = atoi(v);
                    if (col == 4) {
                        ltcT.sosvmin = val;
                        col = 5;
                    }
                    else if (col == 5) {
                        ltcT.sosvmax = val;
                    }
                }
                delete[] v; fflush(stdin);
            }
            if (backToSearch) break; 
        }
        if (backToSearch) { search[0] = '\0'; nIn = 0; ptrLTC = 0; col = 0; continue; } 
        break; 
        delete[] dsGoiY;
    }
    return true;
}


int main()
{
    treeMH rootMH = NULL;
    PTRSV FirstSV = NULL;    
    DSLTC dsLTC; 
    MonHoc mh;     SinhVien sv; 
    int chon; char masv[20];
    
    OpenFileDSLTC(dsLTC, filenameDSLTC);
    OpenFileDSSV(FirstSV, filenameDSSV);
    OpenFileDSMH(rootMH, filenameDSMH);
     
    do {
        chon = Menu(thucdon);
        switch(chon) 
		{
        	case 1: {
                int chonLTC = 0; 
                int so_item_con = 5;
                string vien_con = "+----------------------------------------------------------------+";
                int cot_con_x = (120 - 65) / 2; 
                
                string m_con[] = {
                    "    1. Them lop tin chi",
                    "    2. Hieu chinh lop tin chi",
                    "    3. Xoa lop tin chi",
                    "    4. Liet ke DSLTC",
                    "    5. Thoat ve Menu chinh"
                };
                for(int i=0; i<so_item_con; i++) while(m_con[i].length() < 63) m_con[i] += " ";

                string input_so = "1";
                bool ve_lai_khung = true;

                while(true) {
                    if (ve_lai_khung) {
                        system("cls");
                        SetBGColor(0); SetColor(11);
                        CanGiua("=================== CHON CHUC NANG MANAGEMENT ===================", 2);
                        gotoxy(cot_con_x, 3); cout << vien_con;
                        gotoxy(cot_con_x, 4); cout << "|    NHAP LUA CHON CUA BAN: [   ]                               |";
                        gotoxy(cot_con_x, 5); cout << vien_con;
                        
                        for(int i = 0; i < so_item_con; i++) {
                            gotoxy(cot_con_x, 7 + i); 
                            SetBGColor(0); SetColor(11); cout << "|"; // VIỀN XANH
                            if(i == chonLTC) { 
                                SetBGColor(4); SetColor(15); 
                                cout << m_con[i]; 
                            } else { 
                                SetBGColor(0); SetColor(15); 
                                cout << m_con[i]; 
                            }
                            SetBGColor(0); SetColor(11); cout << "|"; // VIỀN XANH
                        }
                        SetBGColor(0); SetColor(11); gotoxy(cot_con_x, 7 + so_item_con); cout << vien_con;
                        SetColor(14); CanGiua("[UP/DOWN]: Di chuyen  |  [Phim so]: Chon nhanh  |  [ENTER]: Thuc thi", 9 + so_item_con);
                        
                        SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                        ve_lai_khung = false;
                    }

                    int c = _getch();
                    int chon_cu = chonLTC;
                    bool di_chuyen = false;

                    if (c == 0 || c == 224) {
                        c = _getch();
                        if (c == 72) { chonLTC--; if (chonLTC < 0) chonLTC = so_item_con - 1; di_chuyen = true; input_so = to_string(chonLTC + 1); } 
                        else if (c == 80) { chonLTC++; if (chonLTC >= so_item_con) chonLTC = 0; di_chuyen = true; input_so = to_string(chonLTC + 1); }
                    }
                    else if (c >= '1' && c <= '5') {
                        input_so = ""; input_so += (char)c;
                        chonLTC = c - '1'; di_chuyen = true;
                    }
                    else if (c == 8 && input_so.length() > 0) {
                        input_so.pop_back();
                        if(input_so.length() > 0) chonLTC = atoi(input_so.c_str()) - 1;
                        else chonLTC = 0;
                        di_chuyen = true;
                    }
                    else if (c == 27) {
                        SetBGColor(0); SetColor(12); gotoxy(cot_con_x, 11 + so_item_con);
                        CanGiua("Thao tac bi chan! Vui long chon muc 5 de LUU FILE & THOAT.", 11 + so_item_con);
                        Sleep(900); CanGiua("                                                             ", 11 + so_item_con);
                    }
                    else if (c == 13) {
                        int thuc_thi = chonLTC + 1;
                        if (thuc_thi == 5) {
                            system("cls"); SetBGColor(0); SetColor(11);
                            CanGiua("---------------[TIEN HANH GHI DU LIEU LOP TIN CHI VAO FILE]---------------", 5);
                            if (SaveFileDSLTC(dsLTC, filenameDSLTC) == 0) CanGiua("----> Error: Khong the mo file luu!<----", 6);
                            else CanGiua("----> Success: Da luu file danh sach lop tin chi!<----", 6);
                            CanGiua("----------------------------------------------------------", 7);
                            CanGiua("Bam phim bat ky de tro ra Menu chinh...", 9);
                            getch(); break; 
                        }
                        
                        system("cls"); SetBGColor(0); SetColor(7);
                        if (thuc_thi == 1) ThemLTC(dsLTC, rootMH);
                        if (thuc_thi == 2) HieuChinhLTC(dsLTC, rootMH);
                        if (thuc_thi == 3) XoaLTC(dsLTC);
                        if (thuc_thi == 4) LietKeDSLTC(dsLTC, rootMH);
                        
                        ve_lai_khung = true; continue;
                    }

                    if (di_chuyen && chonLTC != chon_cu) {
                        gotoxy(cot_con_x, 7 + chon_cu); 
                        SetBGColor(0); SetColor(11); cout << "|"; 
                        SetBGColor(0); SetColor(15); cout << m_con[chon_cu]; 
                        SetBGColor(0); SetColor(11); cout << "|"; 
                        
                        gotoxy(cot_con_x, 7 + chonLTC); 
                        SetBGColor(0); SetColor(11); cout << "|"; 
                        SetBGColor(4); SetColor(15); cout << m_con[chonLTC]; 
                        SetBGColor(0); SetColor(11); cout << "|"; 
                    }
                    
                    SetBGColor(0); SetColor(15); gotoxy(cot_con_x + 29, 4); cout << "   ";
                    SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                }
                break;
            }
            
            case 2: {
                int chonSV = 0; 
                int so_item_con = 5;
                string vien_con = "+---------------------------------------------------------------+";
                int cot_con_x = (120 - 65) / 2;
                
                string m_con[] = {
                    "    1. Them sinh vien vao lop",
                    "    2. Hieu chinh thong tin sinh vien",
                    "    3. Xoa sinh vien",
                    "    4. In DSSV cua 1 lop (Tang dan ten)",
                    "    5. Thoat ve Menu chinh"
                };
                for(int i=0; i<so_item_con; i++) while(m_con[i].length() < 63) m_con[i] += " ";

                string input_so = "1";
                bool ve_lai_khung = true;

                while(true) {
                    if (ve_lai_khung) {
                        system("cls");
                        SetBGColor(0); SetColor(11);
                        CanGiua("===================== QUAN LY LOP SINH VIEN =====================", 2);
                        gotoxy(cot_con_x, 3); cout << vien_con;
                        gotoxy(cot_con_x, 4); cout << "|    NHAP LUA CHON CUA BAN: [   ]                               |";
                        gotoxy(cot_con_x, 5); cout << vien_con;
                        
                        for(int i = 0; i < so_item_con; i++) {
                            gotoxy(cot_con_x, 7 + i); 
                            SetBGColor(0); SetColor(11); cout << "|"; 
                            if(i == chonSV) { 
                                SetBGColor(4); SetColor(15); 
                                cout << m_con[i]; 
                            } else { 
                                SetBGColor(0); SetColor(15); 
                                cout << m_con[i]; 
                            }
                            SetBGColor(0); SetColor(11); cout << "|"; 
                        }
                        SetBGColor(0); SetColor(11); gotoxy(cot_con_x, 7 + so_item_con); cout << vien_con;
                        SetColor(14); CanGiua("[UP/DOWN]: Di chuyen  |  [Phim so]: Chon nhanh  |  [ENTER]: Thuc thi", 9 + so_item_con);
                        
                        SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                        ve_lai_khung = false;
                    }

                    int c = _getch(); int chon_cu = chonSV; bool di_chuyen = false;

                    if (c == 0 || c == 224) {
                        c = _getch();
                        if (c == 72) { chonSV--; if (chonSV < 0) chonSV = so_item_con - 1; di_chuyen = true; input_so = to_string(chonSV + 1); }
                        else if (c == 80) { chonSV++; if (chonSV >= so_item_con) chonSV = 0; di_chuyen = true; input_so = to_string(chonSV + 1); }
                    }
                    else if (c >= '1' && c <= '5') { input_so = ""; input_so += (char)c; chonSV = c - '1'; di_chuyen = true; }
                    else if (c == 8 && input_so.length() > 0) {
                        input_so.pop_back();
                        if(input_so.length() > 0) chonSV = atoi(input_so.c_str()) - 1;
                        else chonSV = 0;
                        di_chuyen = true;
                    }
                    else if (c == 27) {
                        SetBGColor(0); SetColor(12); 
                        CanGiua(" Thao tac bi chan! Vui long chon muc 5 de LUU FILE & THOAT.", 11 + so_item_con); 
                        Sleep(900); CanGiua("                                                             ", 11 + so_item_con);
                    }
                    else if (c == 13) {
                        int thuc_thi = chonSV + 1;
                        if (thuc_thi == 5) {
                            system("cls"); SetBGColor(0); SetColor(11);
                            CanGiua("---------------[TIEN HANH GHI DU LIEU SINH VIEN VAO FILE]-------------", 5);
                            if (SaveFileDSSV(FirstSV, filenameDSSV) == 0) CanGiua("-> Error: Khong the mo file de luu!", 6);
                            else CanGiua("----> Success: Da luu file danh sach sinh vien thanh cong!<----", 6);
                            CanGiua("----------------------------------------------------------", 7);
                            CanGiua("Bam phim bat ky de tro ra Menu chinh...", 9);
                            getch(); break; 
                        }
                        
                        system("cls"); SetBGColor(0); SetColor(7);
                        if (thuc_thi == 1) NhapSV(FirstSV, sv);
                        if (thuc_thi == 2) HieuChinhSV(FirstSV);
                        if (thuc_thi == 3) XoaSV(FirstSV, dsLTC);
                        if (thuc_thi == 4) InDSSV_1lop_TheoHo_Ten(FirstSV);
                        
                        ve_lai_khung = true; continue;
                    }
                    
                    if (di_chuyen && chonSV != chon_cu) {
                        gotoxy(cot_con_x, 7 + chon_cu); 
                        SetBGColor(0); SetColor(11); cout << "|";
                        SetBGColor(0); SetColor(15); cout << m_con[chon_cu];
                        SetBGColor(0); SetColor(11); cout << "|";
                        
                        gotoxy(cot_con_x, 7 + chonSV); 
                        SetBGColor(0); SetColor(11); cout << "|";
                        SetBGColor(4); SetColor(15); cout << m_con[chonSV];
                        SetBGColor(0); SetColor(11); cout << "|";
                    }

                    SetBGColor(0); SetColor(15); gotoxy(cot_con_x + 29, 4); cout << "   ";
                    SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                }
                break;
            }
            
            case 3: {
                int chonMH = 0; 
                int so_item_con = 5;
                string vien_con = "+---------------------------------------------------------------+";
                int cot_con_x = (120 - 65) / 2;
                
                string m_con[] = {
                    "    1. Them mon hoc",
                    "    2. Hieu chinh mon hoc",
                    "    3. Xoa mon hoc",
                    "    4. In danh sach mon hoc (Tang dan theo ten)",
                    "    5. Thoat ve Menu chinh"
                };
                for(int i=0; i<so_item_con; i++) while(m_con[i].length() < 63) m_con[i] += " ";

                string input_so = "1";
                bool ve_lai_khung = true;

                while(true) {
                    if (ve_lai_khung) {
                        system("cls");
                        SetBGColor(0); SetColor(11);
                        CanGiua("======================= QUAN LY MON HOC =======================", 2);
                        gotoxy(cot_con_x, 3); cout << vien_con;
                        gotoxy(cot_con_x, 4); cout << "|    NHAP LUA CHON CUA BAN: [   ]                               |";
                        gotoxy(cot_con_x, 5); cout << vien_con;
                        
                        for(int i = 0; i < so_item_con; i++) {
                            gotoxy(cot_con_x, 7 + i); 
                            SetBGColor(0); SetColor(11); cout << "|"; 
                            if(i == chonMH) { 
                                SetBGColor(4); SetColor(15); 
                                cout << m_con[i]; 
                            } else { 
                                SetBGColor(0); SetColor(15); 
                                cout << m_con[i]; 
                            }
                            SetBGColor(0); SetColor(11); cout << "|"; 
                        }
                        SetBGColor(0); SetColor(11); gotoxy(cot_con_x, 7 + so_item_con); cout << vien_con;
                        SetColor(14); CanGiua("[UP/DOWN]: Di chuyen  |  [Phim so]: Chon nhanh  |  [ENTER]: Thuc thi", 9 + so_item_con);
                        
                        SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                        ve_lai_khung = false;
                    }

                    int c = _getch(); int chon_cu = chonMH; bool di_chuyen = false;

                    if (c == 0 || c == 224) {
                        c = _getch();
                        if (c == 72) { chonMH--; if (chonMH < 0) chonMH = so_item_con - 1; di_chuyen = true; input_so = to_string(chonMH + 1); }
                        else if (c == 80) { chonMH++; if (chonMH >= so_item_con) chonMH = 0; di_chuyen = true; input_so = to_string(chonMH + 1); }
                    }
                    else if (c >= '1' && c <= '5') { input_so = ""; input_so += (char)c; chonMH = c - '1'; di_chuyen = true; }
                    else if (c == 8 && input_so.length() > 0) {
                        input_so.pop_back();
                        if(input_so.length() > 0) chonMH = atoi(input_so.c_str()) - 1;
                        else chonMH = 0;
                        di_chuyen = true;
                    }
                    else if (c == 27) {
                        SetBGColor(0); SetColor(12); 
                        CanGiua("?? Thao tac bi chan! Vui long chon muc 5 de LUU FILE & THOAT.", 11 + so_item_con); 
                        Sleep(900); CanGiua("                                                             ", 11 + so_item_con);
                    }
                    else if (c == 13) {
                        int thuc_thi = chonMH + 1;
                        if (thuc_thi == 5) {
                            system("cls"); SetBGColor(0); SetColor(11);
                            CanGiua("---------------[TIEN HANH GHI DU LIEU MON HOC VAO FILE]---------------", 5);
                            if (SaveFileDSMH(rootMH, filenameDSMH) == 0) CanGiua("-> Error: Khong the mo file de luu!", 6);
                            else CanGiua("----> Success: Da luu file danh sach mon hoc thanh cong!<----", 6);
                            CanGiua("----------------------------------------------------------", 7);
                            CanGiua("Bam phim bat ky de tro ra Menu chinh...", 9);
                            getch(); break; 
                        }
                        
                        system("cls"); SetBGColor(0); SetColor(7);
                        if (thuc_thi == 1) NhapMH(rootMH);
                        if (thuc_thi == 2) HieuChinhMH(rootMH);
                        if (thuc_thi == 3) XoaMH(dsLTC, rootMH);
                        if (thuc_thi == 4) InDSMH_TangTheoTen(rootMH);
                        
                        ve_lai_khung = true; continue;
                    }
                    
                    if (di_chuyen && chonMH != chon_cu) {
                        gotoxy(cot_con_x, 7 + chon_cu); 
                        SetBGColor(0); SetColor(11); cout << "|";
                        SetBGColor(0); SetColor(15); cout << m_con[chon_cu];
                        SetBGColor(0); SetColor(11); cout << "|";
                        
                        gotoxy(cot_con_x, 7 + chonMH); 
                        SetBGColor(0); SetColor(11); cout << "|";
                        SetBGColor(4); SetColor(15); cout << m_con[chonMH];
                        SetBGColor(0); SetColor(11); cout << "|";
                    }

                    SetBGColor(0); SetColor(15); gotoxy(cot_con_x + 29, 4); cout << "   ";
                    SetBGColor(0); SetColor(12); gotoxy(cot_con_x + 29, 4); cout << input_so;
                }
                break;
            }
            case 4:  { system("cls"); DangKyLTC(dsLTC, FirstSV, rootMH); break; }
            case 5:  { system("cls"); HuyLTC(dsLTC, rootMH); break; }
            case 6:  { system("cls"); NhapSuaDiem(dsLTC, FirstSV, rootMH); break; }
            case 7:  { system("cls"); if (InSVdkLTC(dsLTC, FirstSV, rootMH) == 0) break; getch(); break; }
            case 8:  { system("cls"); InBangDiem(dsLTC, FirstSV, rootMH); break; }
            case 9:  { system("cls"); InDiemTB_Khoa(FirstSV, dsLTC, rootMH); break; }
            case 10: { system("cls"); InBangDiemTongKet(FirstSV, dsLTC, rootMH); break; }
            case 11: {
                system("cls"); SetBGColor(0);
                SaveFileDSSV(FirstSV, filenameDSSV); SaveFileDSMH(rootMH, filenameDSMH); SaveFileDSLTC(dsLTC, filenameDSLTC); 
                SetColor(2); cout << "\n\n";
                CanGiua("+---------------------------------------+", 4);
                CanGiua("|   DA LUU TOAN BO DU LIEU THANH CONG!  |", 5);
                CanGiua("|          THOAT CHUONG TRINH...        |", 6);
                CanGiua("+---------------------------------------+", 7);
                SetColor(7); getch(); return 0;
            }
        }
    } while(1);
    return 0;
}
}
