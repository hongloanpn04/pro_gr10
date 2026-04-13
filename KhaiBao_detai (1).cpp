#include <iostream>
#include <iomanip>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include <vector>
using namespace std;

const int MAX_LTC = 10000;
	char filenameDSLTC[80]="D:\\PR_GR10\\DSLTC.txt";
	char filenameDSSV[200]="D:\\PR_GR10\\DSSV.txt";
	char filenameDSMH[200]="D:\\PR_GR10\\DSMH.txt";
	char filenameDSDK[200]="D:\\PR_GR10\\DSDK.txt"; 

struct MonHoc {
	char MAMH[11];
    char TENMH[51];
	int STCLT, STCTH;
};
struct nodeMH {
	MonHoc mh;
	nodeMH *left, *right;
};
typedef nodeMH* treeMH;



struct DangKy {
	char MASV[16] ; 
    float DIEM; 
    bool huydangky = false;
};
struct nodeDK {
	DangKy dk;
	nodeDK *next;
};
typedef nodeDK* PTRDK;



struct LopTinChi {
    int MALOPTC;
    char  MAMH[11];
    char NienKhoa[10];  
    int Hocky, Nhom, sosvmin, sosvmax;
    bool huylop = false ;
    PTRDK dssvdk = NULL;	
};
struct DSLTC {
	int n = 0;
	LopTinChi* nodes[MAX_LTC];
};
 


struct SinhVien {
	char MASV [16]; char HO[51] ; char TEN[11];
	char  PHAI[4]; char SODT[16]; char MALOP[16];
};
struct nodeSV {
	SinhVien sv;
	nodeSV *next;
};
typedef nodeSV* PTRSV;

const int so_item = 13;
const int dong = 5;
const int cot = 40 ;
char thucdon[so_item][100] ={"1. Mo lop tin chi       ",
			                  "2. In danh sach sinh vien da dang ki lop tin chi             ",
			                  "3. Nhap sinh vien cua 1 lop      ",
			                  "4. In DSSV cua 1 lop(Dua vao ma lop) theo tu tu tang dan ten+ho        ",
			                  "5. Nhap mon hoc         ",
			                  "6. In danh sach mon hoc tang dan theo ten mon hoc            ",
			                  "7. Dang ki lop tin chi  ",
			                  "8. Huy lop tin chi      ",
							  "9. Nhap diem            ", 
							  "10. In bang diem mon hoc cua 1 lop tin chi                   ",
							  "11. In diem trung binh ket thuc khoa hoc cua 1 lop tin chi   ",
							  "12. Bang diem tong ket cac mon cua cac sinh vien trong 1 lop ",
							  "13. Ket thuc chuong trinh          "};

							
// ===== INPUT =====
char* nhap_ma();
char* nhap_chu();
char* nhap_so();
int Menu(char td[so_item][100]);

// ===== MON HOC (BST) =====
int CountNode(treeMH t);
void TreeToArray(treeMH t, MonHoc a[], int &i);
void SortTENMH(MonHoc a[], int n);
void InDSMH_TangTheoTen(treeMH root);
treeMH TimMH(treeMH t, char mamh[]);
int InsertMH(treeMH &t, MonHoc x);
void SaveMH(treeMH t, FILE *f);
int SaveFileDSMH(treeMH root, char *tenfile);
void FreeTree(treeMH &t);
int OpenFileDSMH(treeMH &root, char *tenfile);
int NhapMH(treeMH &root);
bool HieuChinhMH(treeMH &root);
void TimNodeTheMang(treeMH &x, treeMH &y);
int XoaMH(treeMH &t, char mamh[]);

// ===== SINH VIEN =====
PTRSV TimSV(PTRSV FirstSV, char masv[]);
void BaoLoi(const char *s);
void Insert_Order_SV(PTRSV &FirstSV, SinhVien x);
int NhapSV(PTRSV FirstSV, SinhVien &sv, char malop[]);
bool XoaMaSV(PTRSV &FirstSV);
bool HieuChinhSV(PTRSV FirstSV);
int SaveFileDSSV(PTRSV &First, char *tenfile);
int OpenFileDSSV(PTRSV &First, char *tenfile);
void InsertLast(PTRSV &First, SinhVien sv);

// ===== LOP TIN CHI =====
int ThemLTC(DSLTC &ds, treeMH rootMH);
int TimLTC(DSLTC ds, int maloptc);
bool XoaLTC(DSLTC &ds, int maloptc);
int HieuChinhLTC(DSLTC &ds);
int SaveFileDSLTC(DSLTC ds, char *tenfile);
int OpenFileDSLTC(DSLTC &ds, char *tenfile);
void LietKeDSLTC(DSLTC ds);

// ===== DANG KY =====
void InsertLast_DK(PTRDK &First, DangKy dk);
int SaveFileDSDK(DSLTC ds, char *filename);
int OpenFileDSDK(DSLTC &ds, char *filename);
int DemSV_DK(PTRDK First);


// ===== IN DANH SACH =====
void InDSSV_1lop_TheoMa(PTRSV first, char malop[]);
void InDSSV_1lop_TheoHoTen(PTRSV FirstSV, char malop[]);
void InThongtinSV(PTRSV FirstSV, char masv[]);

// ===== GOI Y (CHON) =====
int ChonNienKhoaTheoGoiY(DSLTC &ds, char nienkhoa[]);
int ChonHocKyTheoGoiY(DSLTC &ds, char nk[], int &hk);
int ChonTenMonHocTheoGoiY(DSLTC &ds, treeMH rootMH, char tenmh[], char nk[], int &hk);
int ChonMaLTCTheoGoiY(DSLTC &ds, char nk[], int &hk, char tenMH[], int &maltc, treeMH t);
int ChonMaMonHocTheoGoiY(DSLTC &ds, char mamh[], char nk[], int &hk);
int ChonNhomTheoGoiY(DSLTC &ds, char nk[], int &hk, char maMH[], int &nhom);
int ChonMaLopTheoGoiY(PTRSV &FirstSV, char malop[]);

// ===== CHUC NANG CHINH =====
int  DangKyLopTinChi(DSLTC &ds, PTRSV &FirstSV, treeMH &rootMH);
void HuyLTC(DSLTC &ds, char nienkhoa[], int hocky);
LopTinChi* TimLTC(DSLTC ds, char *nienkhoa, int hocky, char *mamh, int nhom);

// ===== DIEM =====
void NhapSuaDiem(DSLTC &ds, PTRSV FirstSV, treeMH rootMH);
void InBangDiem(PTRSV FirstSV, DSLTC &ds, treeMH rootMH);
void InDiemTongKet(PTRSV FirstSV, DSLTC ds, treeMH rootMH);
void InBangDiemTongKet(PTRSV FirstSV, DSLTC ds, treeMH rootMH);

// ===== KIEM TRA =====
bool TonTaiLop(PTRSV FirstSV, char malop[]);


//a. Mo lop tin chi: cho phep cap nhat(them, xoa, cap nhat) thong tin cua lop tin chi
const short chuoi_so_max = 290;

char* nhap_ma(){
    short dem = 0;
    char *res = new char[chuoi_so_max + 1]; //+1 la ki tu cuoi'\0'
    char tmp;
    while(true){
        tmp = getch();               //doc ki tu tu ban phim
        if( ((tmp >= '0' && tmp <= '9') || (tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z') 
		|| tmp == '-' || tmp == '_') && dem < chuoi_so_max)
        {
            tmp = toupper(tmp);      //viet hoa 
            cout << tmp;
            res[dem++] = tmp;
        } 
        else if(tmp == 8 && dem>0){  //phim backspace && dam bao co ki tu de xoa
            cout << "\b \b";         //lui con tro, ghi de khoang trang, lui lai con tro
			dem--;        
            res[dem] = '\0';         //xoa ki tu vua viet
        }
        else if (tmp == 13 || tmp == 10) { //nhan enter
            res[dem] = '\0';
            cout << endl;
            break;
        }
        else {
            cout << "\a";             //nhap sai se co tieng beep
        }
    }
    return res;
}


char* nhap_chu(){
    short dem = 0;
    char *res = new char[chuoi_so_max + 1]; //+1 la ki tu cuoi'\0'  
    char tmp;
    while(true){
        tmp = getch();              // doc ki tu tu ban phim
        if((tmp == ' ' && dem == 0)) continue; 
		else if(((tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z') || (tmp==' ')) && dem < chuoi_so_max)
        {   //kiem tra co phai chu khong
            if(dem>0 && res[dem-1] == ' ' && tmp == ' ' ) continue;
            cout << tmp;
            res[dem++] = tmp;
        }
		else if(tmp == 8 && dem>0){  // phim backspace
            cout<<"\b \b";          //lui con tro, ghi de khoang trang, lui lai con tro, xoa ki tu vua viet
            res[dem] = '\0';
            dem--;
        }
        else if (tmp == 13 && dem > 0){ //nhan enter
            res[dem] = '\0'; 
            cout << endl;
            break;
        }
        else{
            cout << "\a";                 //nhap sai se co tieng beep
        }
    }
    return res;
}


char* nhap_so(){
    short dem = 0;
    char *res = new char[chuoi_so_max + 1]; //+1 la ki tu cuoi'\0'
    char tmp;
    while(true){
        tmp = getch();              //doc ki tu tu ban phim
        if (tmp >= '0' && tmp <= '9' && dem <chuoi_so_max){
            cout<<tmp;
            res[dem++]=tmp;
        }
        else if(tmp == 8 && dem>0){  //phim backspace
            cout<<"\b \b";          //lui con tro, ghi de khoang trang, lui lai con tro, xoa ki tu vua viet
            res[dem] = '\0';
            dem--;
        }
        else if (tmp == 13 && dem > 0){ //nhan enter
            res[dem] = '\0';           
            cout << endl;
            break;
        }
        else{
            cout<<"\a";             // nhap sai se co tieng beep
        }
    }
    return res;
}


int Menu (char td [so_item][100]){
	system("cls");   int chon =0;
	int i;
	for ( i=0; i< so_item ; i++)
	{ 
		gotoxy(cot, dong +i);
	    cout << td[i];
	}
Nhaplai:
	gotoxy (cot, dong + so_item);
	cout << "Ban chon 1 so (1.."<< so_item << ") :    ";
	int x = wherex();
	int y = wherey();
	char *s = nhap_so();   // ch? nh?p s?
	chon=atoi(s);
	if (chon <1 || chon >so_item)
	{  
		gotoxy(x,y);
	    for (int i = 0; i < strlen(s); i++)
	        cout << " ";
	        
	    gotoxy(x,y);
	
	goto Nhaplai;}
	return chon;
}


//dem so node cua cay
int CountNode(treeMH t){
    if(t == NULL) return 0;
    return 1 + CountNode(t->left) + CountNode(t->right);
}


// chuyen cay -> mang
void TreeToArray(treeMH t, MonHoc a[], int &i){
    if(t == NULL) return;
    TreeToArray(t->left, a, i);
    a[i++] = t->mh;
    TreeToArray(t->right, a, i);
}


// Selection Sort theo TENMH
void SortTENMH(MonHoc a[], int n){
    for(int i = 0; i < n - 1; i++){
        int min = i;
        for(int j = i + 1; j < n; j++){
            if(strcmp(a[j].TENMH, a[min].TENMH) < 0) min = j;
        }
        if(min != i){
            MonHoc temp = a[i];
            a[i] = a[min];
            a[min] = temp;
        }
    }
}


//Ham chinh: In ds mon hoc tang then ten
void InDSMH_TangTheoTen(treeMH root){
    if(root == NULL){
        cout<<"Danh sach rong!\n";
        return;
    }
    // B1: dem so node
    int n = CountNode(root);
    // B2: cap phat dong
    MonHoc *a = new MonHoc[n];
    int i = 0;
    // B3: cay -> mang
    TreeToArray(root, a, i);
    // B4: sap xep theo TENMH
    SortTENMH(a, n);
    // B5: in danh sach
    cout<<"\n\n======DANH SACH MON HOC (TANG DAN THEO TEN)======\n";
    cout<<"      MAMH|                    TENMH|  STCLT|  STCTH|\n";
    for(int i = 0; i < n; i++){
    	printf ("%10s|%25s|%7d|%7d|\n",
        a[i].MAMH, a[i].TENMH, a[i].STCLT, a[i].STCTH);
    }
    // B6: giai phong
    delete[] a;
}

treeMH TimMH(treeMH t, char mamh[]){
    if(t == NULL) return NULL;     //khong tim thay
    int kq = strcmp(mamh, t->mh.MAMH);
    if(kq == 0) 
        return t;        // tim thay
    else if(kq < 0)
        return TimMH(t->left, mamh);   //tim ben trai
    else return TimMH(t->right, mamh);  //tim ben phai
}


//cau a
//cau a
void GoiY_MAMH(treeMH root, char prefix[], char goiy[][11], int &count) {
    if (root == NULL) return;

    GoiY_MAMH(root->left, prefix, goiy, count);

    if (strncmp(root->mh.MAMH, prefix, strlen(prefix)) == 0) {
//        bool trung = false;
//        for (int j = 0; j < count; j++) {
//            if (strcmp(goiy[j], root->mh.MAMH) == 0) {
//                trung = true;
//                break;
//            }
//        }
//
//        if (!trung) {
            strcpy(goiy[count], root->mh.MAMH);
            count++;
        
    }

    GoiY_MAMH(root->right, prefix, goiy, count);
}
int ChonMaMH_TheoGoiY(treeMH root, char mamh[]) {
Nhaplaigoiy:
	char goiy[100][11];
    int count = 0;
    cout << "Nhap ma mon (vi du:INT01->co the nhap I hoac nhan ENTER ,'0 de dung'): ";
    char *prefix = nhap_ma();

    if (strcmp(prefix, "0") == 0) {
        delete[] prefix;
        return 0;
    }

    

    GoiY_MAMH(root, prefix, goiy, count);
    delete[] prefix;

    cout << "\n=== GOI Y MA MON HOC ===\n";

    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << goiy[i] << endl;
    }

    
    if (count == 0) {
        cout << "Khong co ma mon!\n";
    goto Nhaplaigoiy;
    }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;


    if (chon < 1 || chon > count) {
        cout << "Chon sai!\n";
        goto NhapLai;
    }

    strcpy(mamh, goiy[chon - 1]);

    cout << "\nNhap ma mon: " << mamh << endl;

    return 1;
}


int ThemLTC(DSLTC &ds, treeMH rootMH){
	if(ds.n >= MAX_LTC){
		cout<<"Danh sach lop tin chi da day\n";
		return 1;
	}

	LopTinChi *ltc = new LopTinChi;
	cout << " Ma lop tin chi(0 dung nhap): ";
  ltc->MALOPTC = (ds.n == 0) ? 1 : ds.nodes[ds.n-1]->MALOPTC + 1;
	cout << ltc->MALOPTC<<endl;

char mamh[10];
 if (ChonMaMH_TheoGoiY( rootMH,  mamh)==0) return 0;
 strcpy(ltc->MAMH, mamh);
 cout<<"\n";
 Nhapnienkhoa:
	cout << "\nNhap nien khoa(****-****): ";
	char*a = nhap_ma();
	if(strlen(a)==0) 
	goto Nhapnienkhoa;
	strcpy(ltc->NienKhoa, a);
Nhaphocky:
	cout << "\nNhap hoc ky: ";
	char*b=nhap_so();
	if (strcmp(b,"0")==0) 
		{
		cout<<"Hoc ky phai bat dau tu 1. Vui long nhap lai hoc ky!";
		goto Nhaphocky;
		}
	ltc->Hocky=atoi(b);
    //	delete[] b;

Nhaplainhom:
	cout << "\nNhap nhom: ";
	char*c = nhap_so();
	if (strcmp(c,"0")==0) 
	{
		cout<<"Nhom phai bat dau tu 1. Vui long nhap lai nhom!";
		goto Nhaplainhom;
	}
	ltc->Nhom=atoi(c);
	for(int i= 0;i<ds.n;i++ ){
        if ((strcmp (a, ds.nodes[i]->NienKhoa)==0)&& (strcmp(mamh, ds.nodes[i]->MAMH)==0) 
        && (ds.nodes[i]->Hocky==atoi(b))&& (ds.nodes[i]->Nhom==atoi(c))){
            cout<<"Lop tin chi da duoc mo!. Vui long mo lop khac!.";
            goto Nhaplainhom;
        }
    }
	delete[] c;

    cout << "\nNhap so sv min: ";
	char*d = nhap_so();
	ltc->sosvmin = atoi(d);
	delete[] d;
Nhaplaisosvmax:
        cout << "\nNhap so sv max: ";
        char*e = nhap_so();
        ltc->sosvmax = atoi(e);
        delete[] e;
	if (ltc->sosvmax<=ltc->sosvmin)
	{cout<<"So sv max phai lon hon so sv min. Vui long nhap lai!";
		goto Nhaplaisosvmax;
	}


	ltc->huylop = false;
	ltc->dssvdk = NULL;
	ds.nodes[ds.n] = ltc; 
	ds.n++;
	cout << "Them lop tin chi thanh cong!\n";
	getch();
	
	return 1;
}


int TimLTC(DSLTC ds, int maloptc ){
    for(int i=0; i<ds.n; i++){
        if (ds.nodes[i]->MALOPTC == maloptc)
        return i;
    }
    return -1;
}


bool XoaLTC(DSLTC &ds){
 Nhaplaimaloptinchi:
	 int ma;
    cout<<"Nhap ma lop tin chi muon xoa (0 de dung): ";
        char*b=nhap_so();
        if (strcmp(b,"0")==0) return 0;
        ma=atoi(b);

	int pos = TimLTC(ds, ma);
	if(pos == -1){cout<<"Khong co ma lop tin chi phu hop. Vui long nhap lai ma lop tin chi!";
				cout<<"\n";
				goto Nhaplaimaloptinchi;
	}
	delete ds.nodes[pos];
	for(int i = pos; i < ds.n-1; i++){
		ds.nodes[i] = ds.nodes[i+1];
	}
	ds.n--;
	cout<<"Xoa lop tin chi thanh cong!";
		cout<<"\n";
		goto Nhaplaimaloptinchi;
		return true ;


}


int HieuChinhLTC(DSLTC &ds, treeMH &rootMH){     
Nhaplaimaloptinchi: 
	int ma;
    cout<<"Nhap ma lop tin chi muon sua (0 de dung): ";
    char*a=nhap_so();
    ma=atoi(a); 
	 if (ma==0) return 0;         
	int pos = TimLTC(ds, ma);
	if(pos == -1){
		cout << "Khong tim thay lop\n";
		goto Nhaplaimaloptinchi;
		
	}

	LopTinChi *ltc = ds.nodes[pos];
	cout<<"Nhap lai ma mon hoc: ";
	char mamh[10];
 	if (ChonMaMH_TheoGoiY( rootMH,  mamh)==0) return 0;
 	strcpy(ltc->MAMH, mamh);

	 cout<<"\n";
 Nhapnienkhoa:
	cout << "\nNhap lai nien khoa(****-****): ";
	char*u = nhap_ma();
	if(strlen(u)==0) 
	goto Nhapnienkhoa;
	strcpy(ltc->NienKhoa, u);
Nhaphocky:
	cout << "\nNhap lai hoc ky: ";
	char*b=nhap_so();
	if (strcmp(b,"0")==0) 
		{
		cout<<"Hoc ky phai bat dau tu 1. Vui long nhap lai hoc ky!";
		goto Nhaphocky;
		}
	ltc->Hocky=atoi(b);

Nhaplainhom:
	cout << "\nNhap lai nhom: ";
	char*c = nhap_so();
	if (strcmp(c,"0")==0) 
	{
		cout<<"Nhom phai bat dau tu 1. Vui long nhap lai nhom!";
		goto Nhaplainhom;
	}
	ltc->Nhom=atoi(c);
	for(int i= 0;i<ds.n;i++ ){
        if ( i != pos && strcmp (u, ds.nodes[i]->NienKhoa)==0 && strcmp(mamh, ds.nodes[i]->MAMH)==0 && ds.nodes[i]->Hocky==atoi(b)&& ds.nodes[i]->Nhom==atoi(c))
		{
            cout<<"Lop tin chi da duoc mo!. Vui long mo lop khac!.";
            goto Nhaplainhom;
        }
    }
	delete[] c;

    cout << "\nNhap lai so sv min: ";
	char*d = nhap_so();
	ltc->sosvmin = atoi(d);
	delete[] d;
Nhaplaisosvmax:
        cout << "\nNhap lai so sv max: ";
        char*e = nhap_so();
        ltc->sosvmax = atoi(e);
        delete[] e;
	if (ltc->sosvmax<=ltc->sosvmin)
	{cout<<"So sv max phai lon hon so sv min. Vui long nhap lai!";
		goto Nhaplaisosvmax;
	}

	cout<<"Hieu chinh lop tin chi thanh cong!";
	cout<<"\n";
	goto Nhaplaimaloptinchi;
	return 1;
}



//cau b
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
	
int ChonMaLopTheoGoiY(PTRSV &FirstSV, char malop[]){
Nhaplaigoiy:
    cout << "Nhap ma lop(vi du:D24CQCN01 co the nhap D hoac nhan ENTER ,  ('0 de dung'): ";
    char *prefix = nhap_ma();
    if (strcmp(prefix,"0")==0) return 0;

    char goiy[100][10];  //luu truc tiep nien khoa 
    int count = 0;

    cout << "\n=== GOI Y MA LOP ===\n";

    for(PTRSV p = FirstSV; p!=NULL; p=p->next ){
        if(strncmp(p->sv.MALOP, prefix, strlen(prefix)) == 0){

            // kiem tra trung
            bool trung = false;
            for(int j = 0; j < count; j++){
                if(strcmp(goiy[j], p->sv.MALOP) == 0){
                    trung = true;
                    break;
                }
            }

            if(!trung){
                strcpy(goiy[count], p->sv.MALOP);
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
    }

    delete[] prefix;

    if(count == 0){
        cout<<"Khong co ma lop phu hop!\n";
        goto Nhaplaigoiy;
    }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    // l?y k?t qu?
    strcpy(malop, goiy[chon - 1]);
    cout<<"\n";
    cout<<"Nhap ma lop:"<<malop;

    return 1;
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


bool NhapSV(PTRSV &FirstSV, SinhVien &sv){
Nhaplaimalop:
 	char malop[20];
//	ChonMaLopTheoGoiY(FirstSV, malop);
	
	if(	ChonMaLopTheoGoiY(FirstSV, malop)==0) return false;
	else{
	
	if(!TonTaiLop(FirstSV,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}
}
NhaplaiMaSV:
		cout << "\nNhap ma so sinh vien(rong de dung):" ;
		char*s = nhap_ma();
		strcpy(sv.MASV, s);
		delete[] s;
		if (strlen(sv.MASV) == 0) return false;
		
		if (TimSV(FirstSV, sv.MASV) != NULL){
			BaoLoi( "Ma so sinh vien bi trung. Vui long nhap lai. ");
			goto NhaplaiMaSV;
		}

		cout << "Ho sinh vien  :" ; 
		char *t = nhap_chu();                                                         
		strcpy(sv.HO, t);
		delete[] t;	

		cout << "Ten sinh vien :" ; 
		char *b = nhap_chu();
		strcpy(sv.TEN, b );
		delete[] b;

		cout<<"Nhap phai: ";
		char*n = nhap_chu();
		strcpy(sv.PHAI,n);
		delete[] n;

		cout<<"Nhap so dien thoai: ";
		char*e = nhap_so();
		strcpy(sv.SODT, e);
		delete[] e;
		strcpy(sv.MALOP, malop);
		 Insert_Order_SV(FirstSV,sv);
        cout<<"Them sinh vien thanh cong!";
        getch();
        goto NhaplaiMaSV;
		return true;
		
}


void Insert_Order_SV(PTRSV &FirstSV, SinhVien x){
    PTRSV p = new nodeSV;     
    p->sv = x;
    p->next = NULL;

    // dS rong
    if (FirstSV == NULL || strcmp(x.MASV, FirstSV->sv.MASV) <0){   
        p->next = FirstSV;
        FirstSV = p;
        return;
    }
    // Tim vi tri chen
    PTRSV q = FirstSV;
    while (q->next != NULL && strcmp(q->next->sv.MASV, x.MASV) <0)
        q = q->next;

    //  Chen sau q
    p->next = q->next;
    q->next = p;
}


bool XoaMaSV(PTRSV &FirstSV){
Nhaplaimasinhvien:
	cout<<"Nhap MASV can xoa(rong de dung): ";
    char*c=nhap_ma();
	if(strlen(c)==0) return false;	
    SinhVien sv;
    strcpy(sv.MASV,c);
	PTRSV q = TimSV(FirstSV,c);
	if(q == NULL){
		cout<<"Khong tim thay sinh vien\n";
		goto Nhaplaimasinhvien;
	}
	
    PTRSV p = FirstSV, prev = NULL; 
    while(p != NULL && strcmp(p->sv.MASV, c) !=0){
        prev = p;
        p = p->next;
    }
    if(p == NULL){
        return false;
    }
    // xoa dau danh sach
    if(prev == NULL) FirstSV = p->next;
    else prev->next = p->next;
    delete p;
    	cout<<"sinh vien thanh cong!";
		cout<<"\n";
		goto Nhaplaimasinhvien;

    return true;
}

bool HieuChinhSV(PTRSV FirstSV){
NhaplaiMSV:
	cout << "Nhap MASV can hieu chinh (rong de dung): ";
    char*b=nhap_ma();
    if(strlen(b)==0) return false ;
    SinhVien sv;
    strcpy(sv.MASV,b);
	PTRSV p = TimSV(FirstSV,b);
	if(p == NULL){
		cout<<"Khong tim thay sinh vien\n";
		goto NhaplaiMSV;
	}
	cout<<"Nhap ho moi: ";
	char*a=nhap_chu();
	strcpy(p->sv.HO, a);

	cout<<"Nhap ten moi: ";
	char*u=nhap_chu();
	strcpy(p->sv.TEN, u);

	cout<<"Nhap phai: ";
	char*c=nhap_chu();
	strcpy(p->sv.PHAI, c);

	cout<<"Nhap so dien thoai: ";
	char*d=nhap_so();
	strcpy(p->sv.SODT, d);
    cout<<"Hieu chinh sinh vien thanh cong!";
    cout<<"\n";
    goto NhaplaiMSV;
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
    if(kq == 0) return 0;   //trung ma mon 
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
    //Xoa cay cu
    FreeTree(root);

    MonHoc mh;
    while(fscanf(f, "%[^|]|%[^|]|%d|%d\n",
        mh.MAMH,
        mh.TENMH,
        &mh.STCLT,
        &mh.STCTH) == 4){
        InsertMH(root, mh); //ham chen BST theo MAMH
    }
    fclose(f);
//    cout<<"Da doc DSMH thanh cong!\n";
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
        fprintf(f,"%d|%s|%s|%d|%d|%d|%d|%d\n",
            ds.nodes[i]->MALOPTC,
            ds.nodes[i]->MAMH,
            ds.nodes[i]->NienKhoa,
            ds.nodes[i]->Hocky,
            ds.nodes[i]->Nhom,
            ds.nodes[i]->sosvmin,
            ds.nodes[i]->sosvmax,
            ds.nodes[i]->huylop
		);
    }
	fclose (f);
	return 1; 
}

//
//int OpenFileDSLTC(DSLTC &ds, char *tenfile){
//    FILE *f = fopen(tenfile,"r");
//    if(f == NULL){ 
//        cout << "Mo file THAT BAI!\n";
//	    return 0; 
//    }  
//
//    LopTinChi ltc;
//    ds.n = 0;     //xoa ds cu
//    while(fscanf(f,"%d %10s %9s %d %d %d %d %d",
//        &ltc.MALOPTC,
//        ltc.MAMH,
//        ltc.NienKhoa,
//        &ltc.Hocky,
//        &ltc.Nhom,
//        &ltc.sosvmin,
//        &ltc.sosvmax,
//        &ltc.huylop) == 8){
//        LopTinChi *p = new LopTinChi;
//        *p = ltc;
//
//        ds.nodes[ds.n] = p;
//        ds.n++;
//    }
//    fclose(f);
//    return 1;
//}

int OpenFileDSLTC(DSLTC &ds, char *tenfile){
    FILE *f = fopen(tenfile,"r");
    if(f == NULL){ 
        cout << "Mo file THAT BAI!\n";
	    return 0; 
    }  

    LopTinChi ltc;
    ds.n = 0;     //xoa ds cu
    while(fscanf(f,"%d|%[^|]|%[^|]|%d|%d|%d|%d|%d\n",
        &ltc.MALOPTC,
        ltc.MAMH,
        ltc.NienKhoa,
        &ltc.Hocky,
        &ltc.Nhom,
        &ltc.sosvmin,
        &ltc.sosvmax,
        &ltc.huylop) == 8){
        LopTinChi *p = new LopTinChi;
        *p = ltc;

        ds.nodes[ds.n] = p;
        ds.n++;
    }
    fclose(f);
    return 1;
}


void LietKeDSLTC (DSLTC ds) {
	int dem = 0; 
    system ("cls");
	printf ("MALOPTC|      MAMH|     NienKhoa|  Hocky|   Nhom|   Sosvmin|   Sosvmax|   Huylop|\n");
	for (int i=0; i<ds.n; i++){
		printf ("%7d|%10s|%13s|%7d|%7d|%10d|%10d|%9d|\n",
	        ds.nodes[i]->MALOPTC, ds.nodes[i]->MAMH,  ds.nodes[i]->NienKhoa, ds.nodes[i]->Hocky, 
            ds.nodes[i]->Nhom, ds.nodes[i]->sosvmin, ds.nodes[i]->sosvmax, ds.nodes[i]->huylop);
		dem ++;
	}
	cout << "\nSo luong LTC =" << dem;

}


void InDSSV_1lop_TheoMa(PTRSV first){
Nhaplaimalop:
 	char malop[20];
	ChonMaLopTheoGoiY(first, malop);
	
	if(strlen(malop)==0) return;
	if(!TonTaiLop(first,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}
	
    if(first == NULL) {
        cout<<"Danh sach rong!";
        return;
    }

    for(PTRSV p = first; p != NULL; p = p->next){
        for(PTRSV q = p->next; q != NULL; q = q->next){
            //chi xet sinh vien cung lop
            if(strcmp(p->sv.MALOP, malop) == 0 && 
            strcmp(q->sv.MALOP, malop) == 0){
                if(strcmp(p->sv.MASV, q->sv.MASV) > 0){
                    SinhVien temp = p->sv;
                    p->sv = q->sv;
                    q->sv = temp;
                }
            }
        }
    }

    // in danh sach
    cout<<"\n\n                        ======DANH SACH SINH VIEN THEO TEN HO======           \n";
    cout<<"            MASV|                                      HO|        TEN|    PHAI|            SODT|\n";
    
    bool found = false;
    for(PTRSV p = first; p != NULL; p = p->next){
        if(strcmp(p->sv.MALOP, malop) == 0){
            found = true;
            printf("%16s|%40s|%11s|%8d|%16d|\n",
	        p->sv.MASV, p->sv.HO, p->sv.TEN,
	        p->sv.PHAI,p->sv.SODT);
        }
    }
    if (!found) cout<<"Khong tim thay sinh vien trong lop nay!";
}


void InDSSV_1lop_TheoHo_Ten(PTRSV FirstSV){
Nhaplaimalop:
 	char malop[20];
	ChonMaLopTheoGoiY(FirstSV, malop);
	
	if(strlen(malop)==0) return;
	if(!TonTaiLop(FirstSV,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}
	
    int i=0,n = 0;

    // loc sinh vien theo lop
    for(PTRSV p = FirstSV; p != NULL; p = p->next)
        if(strcmp(p->sv.MALOP, malop) == 0) n++;

	PTRSV *arr= new PTRSV[n];
	//copy
	for(PTRSV p = FirstSV; p != NULL; p = p->next){
        if(strcmp(p->sv.MALOP, malop) == 0){
            arr[i] = p;
    		i++;
        }
    }
    
    if(n == 0){
        cout<<"Khong co sinh vien trong lop\n";
        return;
    }

    // sap xep theo TEN + HO
    for(int i = 0; i < n-1; i++)
    for(int j = i+1; j < n; j++){
        if(strcmp(arr[i]->sv.TEN, arr[j]->sv.TEN) > 0 ||
           (strcmp(arr[i]->sv.TEN, arr[j]->sv.TEN) == 0 &&
            strcmp(arr[i]->sv.HO, arr[j]->sv.HO) > 0))
        {
            PTRSV temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // in danh sach
    cout <<"\n\n                            ======DANH SACH SINH VIEN THEO TEN HO======                \n";
    cout <<"            MASV|                                      HO|        TEN|    PHAI|            SODT|\n";
    
    for(int i = 0; i < n; i++){
    	printf("%16s|%40s|%11s|%8d|%16d|\n",
        arr[i]->sv.MASV, arr[i]->sv.HO, arr[i]->sv.TEN,
        arr[i]->sv.PHAI,arr[i]->sv.SODT);
    }
}
bool TrungTenMH(treeMH t, char ten[]){
    if(t == NULL) return false;

    if(strcmp(t->mh.TENMH, ten) == 0)
        return true;

    return TrungTenMH(t->left, ten) || TrungTenMH(t->right, ten);
}



int NhapMH(treeMH &root){
    MonHoc mh;
NhaplaiMaMH:
	cout << "\nNhap ma mon hoc(rong de dung):";
	char*s = nhap_ma();
	strcpy(mh.MAMH, s);
	delete[] s;
	if (strlen(mh.MAMH) == 0) return 0;
	if (TimMH(root, mh.MAMH) != NULL){
	    BaoLoi("Ma mon hoc bi trung. Ban nhap lai. ");
	    goto NhaplaiMaMH;
	}
   Nhaplaitenmh:
    cout << "Nhap ten mon hoc: ";
    char *b = nhap_ma();
    if(TrungTenMH(root, b)){
    cout << "Ten mon hoc da ton tai!\n";
    goto Nhaplaitenmh;
}
	strcpy(mh.TENMH,b);
Nhaplaisotclt:
    cout<<"Nhap so tin chi ly thuyet: ";
    char *c = nhap_so();
    int m=atoi(c);
    if (m<=0) {
    	goto Nhaplaisotclt;
	}
    mh.STCLT = atoi(c);
    delete[] c;
Nhaplaisotcth:
    cout<<"Nhap so tin chi thuc hanh: ";
    char *d = nhap_so();
     int n=atoi(c);
    if (n<=0) {
    	goto Nhaplaisotcth;
	}
    mh.STCTH = atoi(d);
    delete[] d;

    InsertMH(root,mh);
    cout << "Them mon hoc thanh cong\n";
    return 1;
}



bool HieuChinhMH(treeMH &root){
Nhaplai:
    cout<<"Nhap ma mon hoc can hieu chinh (0 de dung): ";
    char *a = nhap_ma();
	if (strcmp(a,"0")==0) return false;
    treeMH p = TimMH(root,a);

    if(p == NULL){
        cout << "Khong tim thay mon hoc\n";
        goto Nhaplai;
    }
Nhaplaitenmh:
    cout << "Nhap ten mon hoc moi: ";
    char *b = nhap_ma();
    if(TrungTenMH(root, b)){
    cout << "Ten mon hoc da ton tai!\n";
    goto Nhaplaitenmh;
}
    strcpy(p->mh.TENMH,b);
Nhaplaisotclt:

    cout<<"Nhap STCLT moi: ";
    char *c = nhap_so();
      int n=atoi(c);
    if (n<=0) {
    	goto Nhaplaisotclt;
	}
    
    p->mh.STCLT = atoi(c);
Nhaplaisotcth:
    cout<<"Nhap STCTH moi: ";
    char *d = nhap_so();
    int m=atoi(d);
    if (m<=0) {
    	goto Nhaplaisotcth;
	}
    p->mh.STCTH = atoi(d);

    cout<<"Cap nhat mon hoc thanh cong\n";
    goto Nhaplai;
    return true;
  
}


void TimNodeTheMang(treeMH &x, treeMH &y){
    if(y->left != NULL)
        TimNodeTheMang(x,y->left);
    else{
        x->mh = y->mh;
        x = y;
        y = y->right;
    }
}


int XoaMH(treeMH &t, char mamh[]) {
 
    if (t == NULL) {
        return 0; 
    }

    // So sanh ma nhap vao voi ma tai node hien tai
    int kq = strcmp(mamh, t->mh.MAMH);

    if (kq < 0) {
        // Neu nho hon, tiep tuc tim ben trai
        return XoaMH(t->left, mamh);
    } 
    else if (kq > 0) {
        // Neu lon hon, tiep tuc tim ben phai
        return XoaMH(t->right, mamh);
    } 
    else {
        // Da tim thay mon hoc (kq == 0)
        nodeMH *p = t; // Bien tam de giai phong bo nho

        if (t->left == NULL) {
            // Truong hop node khong co con trai
            t = t->right;
        } 
        else if (t->right == NULL) {
            // Truong hop node khong co con phai
            t = t->left;
        } 
        else {
            // Truong hop co ca 2 con: Tim node the mang (trai nhat cua con phai)
            TimNodeTheMang(p, t->right);
        }

        delete p; // Xoa node khoi bo nho
        return 1; 
        
    }
}


// Dung cho g
void InThongtinSV(PTRSV FirstSV, char masv[]){
	PTRSV p=TimSV( FirstSV,  masv);

	cout<<"\n            MASV|                                      HO|        TEN|    PHAI|            SODT|\n";
	printf("%16s|%40s|%11s|%8d|%16d|\n",
        p->sv.MASV, p->sv.HO, p->sv.TEN,
        p->sv.PHAI, p->sv.SODT);
}


int DemSV_DK(PTRDK First){
    int dem = 0;
    PTRDK p = First;
    while (p != NULL) {
        if (p->dk.huydangky == false){  //chi tinh SV chua huy
            dem++;
        }
        p = p->next;
    }
    return dem;
}


int SaveFileDSDK(DSLTC ds, char *filename){
    FILE *f = fopen(filename, "w");
    if(f == NULL) return 0;

    for(int i = 0; i < ds.n; i++){
        PTRDK p = ds.nodes[i]->dssvdk;
        while(p != NULL){
            fprintf(f, "%d|%s|%f|%d\n",
                ds.nodes[i]->MALOPTC,
                p->dk.MASV,
                p->dk.DIEM,
                p->dk.huydangky);
            p = p->next;
        }
    }
    fclose(f);
    return 1;
}

int ChonNienKhoaTheoGoiY(DSLTC &ds, char nienkhoa[]){
Nhaplaigoiy:
    cout << "Nhap nien khoa (vi du:2024-2025 co the nhap 2 hoac nhan ENTER,  '0 de dung'): ";
    char *prefix = nhap_ma();
    if (strcmp(prefix,"0")==0) return 0;

    char goiy[100][10];  // luu truc tiep nien khoa 
    int count = 0;

    cout << "\n=== GOI Y NIEN KHOA ===\n";

    for(int i = 0; i < ds.n; i++){
        if(strncmp(ds.nodes[i]->NienKhoa, prefix, strlen(prefix)) == 0){

            // kiem tra trung
            bool trung = false;
            for(int j = 0; j < count; j++){
                if(strcmp(goiy[j], ds.nodes[i]->NienKhoa) == 0){
                    trung = true;
                    break;
                }
            }

            if(!trung){
                strcpy(goiy[count], ds.nodes[i]->NienKhoa);
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
    }

    delete[] prefix;

    if(count == 0){
        cout<<"Khong co nien khoa phu hop!\n";
        goto Nhaplaigoiy;
    }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    //lay kq 
    strcpy(nienkhoa, goiy[chon - 1]);
    cout<<"\n";
    cout<<"Nhap nien khoa:"<<nienkhoa;

    return 1;
}
int ChonHocKyTheoGoiY(DSLTC &ds, char nk[], int &hk) {
	
	int goiy[100];  
    int count = 0;

    cout << "\n=== GOI Y HOC KY ===\n";

    for(int i = 0; i < ds.n; i++){
        if( strcmp(ds.nodes[i]->NienKhoa, nk )==0) {
        	
            // kiem tra trung
            bool trung = false;
            for(int j = 0; j < count; j++){
                if(goiy[j]==ds.nodes[i]->Hocky){
                    trung = true;
                    break;
                }
            }
           
        if(!trung) 
			{	goiy[count]=ds.nodes[i]->Hocky;
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
	}

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    //lay ket qua 
   hk=goiy[chon - 1];
    cout<<"Nhap hoc ki:"<<hk ;

    return 1;
}
int ChonTenMonHocTheoGoiY(DSLTC &ds, treeMH rootMH, char tenmh[], char nk[], int& hk){
Nhaplaigoiy:
    cout << "Nhap goi y ten mon hoc (vi du: mon CTDL ->nhap C hoac nhan ENTER, ('0 de dung')): ";
    char *prefix = nhap_ma();
    if (strcmp (prefix,"0")==0) return 0;

    char goiy[100][10];  // luu truc tiep nien khoa 
    int count = 0;

    cout << "\n=== GOI Y TEN MON HOC ===\n";

    for(int i = 0; i < ds.n; i++){
    	treeMH q=TimMH(rootMH, ds.nodes[i]->MAMH);
    	if (q!=NULL)
    	{
		 	if( strcmp(ds.nodes[i]->NienKhoa, nk )==0 && ds.nodes[i]->Hocky==hk 
			 && strncmp(q->mh.TENMH, prefix, strlen(prefix)) == 0){

            // kiem tra trùng
            	bool trung = false;
            	for(int j = 0; j < count; j++){
                	if(strcmp(goiy[j],q->mh.TENMH) == 0){
                    	trung = true;
                   	 break;
                	}
            	}
  		

            if(!trung){
                strcpy(goiy[count], q->mh.TENMH);
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
    }
}

    delete[] prefix;

    if(count == 0){
        cout<<"Khong co ten mon hoc phu hop!\n";
        goto Nhaplaigoiy;
    }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    // l?y k?t qu?
    strcpy(tenmh, goiy[chon - 1]);
    cout<<"\n";
    cout<<"Ten mon hoc:"<<tenmh;

    return 1;
}


int ChonMaLTCTheoGoiY(DSLTC &ds, char nk[], int & hk, char tenMH[], int &maltc,treeMH t) {
    int goiy[100];  
    int count = 0;

    cout << "\n=== GOI Y MA LOP TIN CHI ===\n";

    for(int i = 0; i < ds.n; i++){
    	treeMH p=TimMH( t,ds.nodes[i]->MAMH);
    	if (p!=NULL)
    	{
       		if( ds.nodes[i]->huylop==false && (ds.nodes[i]->NienKhoa, nk )==0 && ds.nodes[i]->Hocky==hk && strcmp(p->mh.TENMH, tenMH) == 0){
           
                goiy[count]=ds.nodes[i]->MALOPTC;
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
    }
if (count == 0) {
        cout << "Khong tim thay lop tin chi nao phu hop!" << endl;
        return 0;
    }


NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    // lay kq 
   maltc=goiy[chon - 1];
    cout<<"Nhap ma lop tin chi:"<<maltc ;

    return 1;
}

int  DangKyLopTinChi(DSLTC &ds, PTRSV &FirstSV, treeMH &rootMH) {
Nhaplaimasinhvien:
   cout << "\nNhap ma sinh vien (rong de dung): ";
    char *s=nhap_ma();
    if (strlen(s)==0) return 0;
    PTRSV p = TimSV(FirstSV, s);
    	if(p==NULL){ 
            cout<<"Khong tim thay sinh vien!.";
    		goto Nhaplaimasinhvien;
        	}

    InThongtinSV(FirstSV, s);

    char Nienkhoa[10];
	if (ChonNienKhoaTheoGoiY( ds, Nienkhoa)==0) return 0;
 	cout<<"\n";
	int hocki;
	ChonHocKyTheoGoiY( ds,Nienkhoa,hocki);
	cout<<"\n";
	cout << "\n====DANH SACH LOP TIN CHI====\n";
    bool found = false;
     //liet ke danh sach lop tin chi
    for (int i = 0; i < ds.n; i++){
		int soSVdk = DemSV_DK(ds.nodes[i]->dssvdk);
        if (strcmp(ds.nodes[i]->NienKhoa, 	Nienkhoa) == 0 &&
            (ds.nodes[i]->Hocky== hocki) && (ds.nodes[i]->huylop == false)) {
            	found = true;
            treeMH mhNode=TimMH(rootMH, ds.nodes[i]->MAMH);
			// dem so luong sinh vien dang khi ko tinh sinh vien da huy
			 if (mhNode!=NULL){
    		cout << "\nMa lop: " << ds.nodes[i]->MALOPTC;
            cout << " | Ma MH: " << ds.nodes[i]->MAMH;
            cout << " | Ten MH: "<<mhNode->mh.TENMH;
            cout << " | Nhom: " << ds.nodes[i]->Nhom;
            cout << " | Da DK: " << soSVdk;
            cout << " | Con trong: " << ds.nodes[i]->sosvmax - soSVdk;
            cout<<"\n";
        }
	}
}
	if (!found) {cout<<"Khong co lop tin chi nao duoc mo!";
				getch();
				return 0;
				}
Nhaplaitenmonhoc:	
	char tenmh[51];
	if (ChonTenMonHocTheoGoiY(ds, rootMH,  tenmh, Nienkhoa,  hocki)==0) return 0;
	cout << "\n====DANH SACH LOP TIN CHI====\n";
     //liet ke danh sach lop tin chi
    for (int i = 0; i < ds.n; i++){
    	int soSVdk = DemSV_DK(ds.nodes[i]->dssvdk);
    	treeMH mhNode=TimMH(rootMH, ds.nodes[i]->MAMH);
    	if (mhNode!=NULL) 
    	{
    		if (strcmp(ds.nodes[i]->NienKhoa, 	Nienkhoa) == 0 &&
            (ds.nodes[i]->Hocky== hocki) && (ds.nodes[i]->huylop == false) && strcmp(mhNode->mh.TENMH, tenmh)==0) {
       		// dem so luong sinh vien dang khi ko tinhs sinh vien da huy
    		cout << "\nMa lop: " << ds.nodes[i]->MALOPTC;
            cout << " | Ma MH: " << ds.nodes[i]->MAMH;
            if (mhNode!=NULL)
            cout << " | Ten MH: "<<mhNode->mh.TENMH;
            cout << " | Nhom: " << ds.nodes[i]->Nhom;
            cout << " | Da DK: " << soSVdk;
            cout << " | Con trong: " << ds.nodes[i]->sosvmax - soSVdk;
            cout<<"\n";
        }
	}
}
int dem = 0;
int maloptc;
ChonMaLTCTheoGoiY(ds, Nienkhoa, hocki,  tenmh, maloptc,rootMH);
	cout<<"\n";
    if (maloptc==0) {
    	if(dem >0) cout << "Da ghi sinh vien dang ki vao file."; 
		else cout<<"Sinh vien chua dang ki lop mon hoc";
        delete[] s;
        return 0;
    }
    
    // Tim lop tin chi theo dieu kien
    bool foundLTC=false;
    for (int i = 0; i < ds.n; i++) 
    	
        {if (ds.nodes[i]->MALOPTC == maloptc && strcmp(ds.nodes[i]->NienKhoa, Nienkhoa)==0
		&&ds.nodes[i]->Hocky==hocki && ds.nodes[i]->huylop==false) {
		 	foundLTC=true;
            int soSVdk = DemSV_DK(ds.nodes[i]->dssvdk);

            // Check full
            if (soSVdk >= ds.nodes[i]->sosvmax) {
                cout << "Lop da day!\n";
                return -1 ;
            }

            // Check tring
            PTRDK p = ds.nodes[i]->dssvdk;

            while (p != NULL) {
                if (strcmp(p->dk.MASV, s) == 0 && p->dk.huydangky == false  ) {
                	
                	if(p->dk.DIEM > 0) cout<<"Sinh vien da hoan thanh mon hoc nay!\n";
                	else cout<<"Sinh vien da dang ky mon hoc nay roi!\n";
                
                    goto Nhaplaitenmonhoc;
                }
                p = p->next;
            }

            //Kiem tra da dang ki 1 nhom cua mon hoc
            for (int j=0; j<ds.n; j++){
            	if (strcmp(ds.nodes[j]->MAMH,ds.nodes[i]->MAMH)==0 ){
				
            		PTRDK q=ds.nodes[j]->dssvdk;
            		while (q!=NULL){
                        if( strcmp(q->dk.MASV,s)==0 && q->dk.huydangky==false){
                            cout<<"Sinh vien da dang ki 1 nhom cua mon hoc nay roi!.";
                            goto Nhaplaitenmonhoc;
					    }
				        q=q->next;
				    }
			    }
		    }

            // Them dang ky 
            PTRDK newDK = new nodeDK;
            // them masv
            strcpy(newDK->dk.MASV, s);
            // them diem
            newDK->dk.DIEM = 0;
            // dang ki
            newDK->dk.huydangky = false;
            // tao
            newDK->next= ds.nodes[i]->dssvdk;

            ds.nodes[i]->dssvdk = newDK;
        
			dem++;
            cout << "Dang ky thanh cong!\n";
            SaveFileDSDK(ds, filenameDSDK);
            
            goto Nhaplaitenmonhoc;  
            
            return 1;
        }
    }
    if (!foundLTC) {
        cout<<"Ma lop tin chi khong hop le!.";
		goto Nhaplaitenmonhoc;
	}
}


int ChonMaMonHocTheoGoiY(DSLTC &ds, char mamh[], char nk[], int& hk){

Nhaplaigoiy:
    cout << "Nhap goi y ma mon hoc (vi du: ma INT01->nhap I hoac nhan ENTER): ";
    char *prefix = nhap_ma();

    char goiy[100][10];  // luu tr?c ti?p niên khóa
    int count = 0;

    cout << "\n=== GOI Y MA MON HOC ===\n";

    for(int i = 0; i < ds.n; i++){
        if( strcmp(ds.nodes[i]->NienKhoa, nk )==0 && ds.nodes[i]->Hocky==hk 
		&& strncmp(ds.nodes[i]->MAMH, prefix, strlen(prefix)) == 0){

            // ki?m tra trùng 
            bool trung = false;
            for(int j = 0; j < count; j++){
                if(strcmp(goiy[j], ds.nodes[i]->MAMH) == 0){
                    trung = true;
                    break;
                }
            }

            if(!trung){
                strcpy(goiy[count], ds.nodes[i]->MAMH);
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }
    }

    delete[] prefix;

    if(count == 0){
        cout<<"Khong co ma mon hoc phu hop!\n";
        goto Nhaplaigoiy;
    }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    // l?y k?t qu?
    strcpy(mamh, goiy[chon - 1]);
    cout<<"\n";
    cout<<"Ma mon hoc:"<<mamh;

    return 1;
}

int ChonNhomTheoGoiY(DSLTC &ds, char nk[], int &hk, char maMH[], int &nhom) {
    int goiy[100];  
    int count = 0;

    cout << "\n=== GOI Y NHOM ===\n";

    for(int i = 0; i < ds.n; i++){
        if( strcmp(ds.nodes[i]->NienKhoa, nk )==0 && ds.nodes[i]->Hocky==hk 
		&& strcmp(ds.nodes[i]->MAMH, maMH) == 0){
           
                goiy[count]=ds.nodes[i]->Nhom;
                cout << count + 1 << ". " << goiy[count] << endl;
                count++;
            }
        }

NhapLai:
    cout << "Chon: ";
    char *s = nhap_so();
    int chon = atoi(s);
    delete[] s;

    if(chon < 1 || chon > count){
        cout<<"Chon sai!\n";
        goto NhapLai;
    }

    // l?y k?t qu?
   nhom=goiy[chon - 1];
    cout<<"Nhap nhom:"<<nhom  ;

    return 1;
}

bool InSVdkLTC(DSLTC &ds, PTRSV &FirstSV, treeMH &rootMH){
 	char nienkhoa[10];
	if (ChonNienKhoaTheoGoiY(ds,nienkhoa)==0) return false ;
	cout<<"\n";
	int hocky;
	ChonHocKyTheoGoiY( ds,nienkhoa,hocky);
	cout<<"\n";
	system("cls");
    for (int i=0; i<ds.n; i++){ //goi nho
    	if (strcmp(ds.nodes[i]->NienKhoa, nienkhoa)==0 && ds.nodes[i]->Hocky==hocky)
    	{
    		treeMH mh=TimMH(rootMH, ds.nodes[i]->MAMH);
    		if (mh!=NULL){
    			cout<<"Ma mon hoc:"<<mh->mh.MAMH
    			    <<"| Ten mon hoc:"<<mh->mh.TENMH
    			    <<"| Nhom:"<<ds.nodes[i]->Nhom<<endl;
			}

		}
	}
	char mamh[10];
	if (ChonMaMonHocTheoGoiY(ds, mamh,nienkhoa,hocky)==0) return false ;
	cout<<"\n";
	int nhom;
	ChonNhomTheoGoiY(ds,nienkhoa,hocky, mamh,nhom);


    for(int i = 0; i < ds.n; i++){
        if(strcmp(ds.nodes[i]->NienKhoa, nienkhoa) == 0 && strcmp(ds.nodes[i]->MAMH, mamh) == 0 
            && ds.nodes[i]->Hocky == hocky &&
            ds.nodes[i]->Nhom == nhom){
                cout<<"\n               ===DANH SACH SINH VIEN DANG KY===\n";
                cout<<"            MASV|                     HO TEN|      TRANG THAI|\n";

            PTRDK p = ds.nodes[i]->dssvdk;
            if(p == NULL){
                cout<<"Chua co sinh vien dang ky!\n";
                getch();
                return false;
            }

            while(p != NULL){
                //lay thong tin sv
                PTRSV sv = TimSV(FirstSV, p->dk.MASV);

                if(sv != NULL){
                    cout << setw(16) << sv->sv.MASV << "|"
		             	 << setw(25) << sv->sv.HO << " " << sv->sv.TEN << "|";
                    if(p->dk.huydangky) cout << setw(16) << "DA HUY" << "|\n";
                    else cout << setw(16) << "DANG KY" << "|\n";
                }
                p = p->next;
            }
        }
    }
    return true;
   
  }


//cau h
void HuyLTC(DSLTC &ds, char nienkhoa[], int hocky){
    int dsHuy[MAX_LTC];
    int dem;
    for(int i = 0; i < ds.n; i++){
        LopTinChi* ltc = ds.nodes[i];
        if (strcmp(ltc->NienKhoa, nienkhoa) == 0 &&
        (ltc->Hocky== hocky) && (ltc->huylop == false)) {
            int soSVdk = DemSV_DK(ltc->dssvdk);
            if(soSVdk < ltc->sosvmin){
                dsHuy[dem++] = i;
            }
        }
    }
    if(dem == 0){
        cout << "Khong co lop nao bi huy\n";
        return;
    }
    cout << "Danh sach lop se bi huy:\n";
    for(int i = 0; i < dem; i++){
        int idx = dsHuy[i];
        cout << "Ma lop TC: " << ds.nodes[idx]->MALOPTC
             << " | So SV: " << DemSV_DK(ds.nodes[idx]->dssvdk) << endl;
    }

    cout << "Ban co chac muon huy? (y/n): ";
    char ch;
    cin >> ch;
    if(ch != 'y' && ch != 'Y'){
        cout << "Da huy thao tac!\n";
        return;
    }
    for(int i = 0; i < dem; i++){
        int idx = dsHuy[i];
        ds.nodes[idx]->huylop = true;
    }
    cout << "Huy lop thanh cong!\n";
}

LopTinChi* TimLTC(DSLTC ds, char *nienkhoa, int hocky, char *mamh, int nhom){
    for(int i=0; i<ds.n; i++){
        LopTinChi* ltc = ds.nodes[i];
        if(strcmp(ltc->NienKhoa, nienkhoa) == 0 && ltc->Hocky==hocky 
            && strcmp(ltc->MAMH, mamh) == 0 && ltc->Nhom == nhom){
                return ltc;
        }
    }
    return NULL; 
}


//cau i
void NhapSuaDiem(DSLTC &ds, PTRSV FirstSV, treeMH rootMH){
	char nienkhoa[10];
	if (ChonNienKhoaTheoGoiY(ds,nienkhoa)==0) return ;
	cout<<"\n";
	int hocky;
	ChonHocKyTheoGoiY( ds,nienkhoa,hocky);
	cout<<"\n";
    for (int i=0; i<ds.n; i++){ //goi nho
    	if (strcmp(ds.nodes[i]->NienKhoa, nienkhoa)==0 && ds.nodes[i]->Hocky==hocky)
    	{
    		treeMH mh=TimMH(rootMH, ds.nodes[i]->MAMH);
    		if (mh!=NULL){
    			cout<<"Ma mon hoc:"<<mh->mh.MAMH
    			    <<"| Ten mon hoc:"<<mh->mh.TENMH
    			    <<"| Nhom:"<<ds.nodes[i]->Nhom<<endl;
			}
		}
	}
	char mamh[10];
	if (ChonMaMonHocTheoGoiY(ds, mamh,nienkhoa,hocky)==0) return ;
	cout<<"\n";
	int nhom;
	ChonNhomTheoGoiY(ds,nienkhoa,hocky, mamh,nhom);

	LopTinChi* ltc = TimLTC(ds, nienkhoa, hocky, mamh, nhom);
    if(ltc == NULL){
        cout<<"Khong tim thay lop tin chi!\n";
        getch();  
        return;
    }
    
    // Ðua dsdk -> vector
    vector<PTRDK> list;
    PTRDK p = ltc->dssvdk;
    while(p != NULL){
        if(!p->dk.huydangky)
            list.push_back(p);
        p = p->next;
    }

    int n = list.size();
    if(n == 0){
        cout<<"Khong co sinh vien!\n";
        getch();
        return;
    }
	
    int row = 0;

    while(true){
        system("cls");
        cout << "NOTE:\n" 
		<< "Nhan ESC de luu diem.\n" 
		<< "Nhan ENTER de nhap diem.\n" 
		<< "Nhan Up:^ hoac Down:v de dieu khien cac dong" <<endl;
        cout << "                           =====DANH SACH SINH VIEN DA DANG KY===== \n" ;
		cout<< setw(9)  << "STT"
     		<< " | " << setw(16) << "MASV"
     		<< " | " << setw(25) << "HO"
     		<< " | " << setw(25) << "TEN"
     		<< " | " << setw(7)  << "DIEM"
     		<< " |\n";

        for(int i = 0; i < n; i++){
            PTRSV sv = TimSV(FirstSV, list[i]->dk.MASV);

            if(i == row){
			    cout << ">>";
			    SetColor(2); //green 
			}else{
			    cout << "  ";
			    SetColor(7); //white 
			}

            if(sv != NULL){
                cout << setw(7) << i+1 
     			<< " | " << setw(16) << sv->sv.MASV
     			<< " | " << setw(25) << sv->sv.HO
     			<< " | " << setw(25) << sv->sv.TEN
     			<< " | ";

			if(list[i]->dk.DIEM >= 0)
    			cout << right << setw(7) << list[i]->dk.DIEM;
			else
    			cout << setw(5) << "";

			cout << " |\n";
            }
            SetColor(7); 
        }
		
        char c = getch();
        // phim dac biet 
        if(c == -32){
            char key = getch();

            if(key == 72){ // tro len 
                if(row > 0) row--;
            }
            else if(key == 80){ // tro xuong 
                if(row < n-1) row++;
            }
        }
        
        else if(c == 13){ // ENTER
	    gotoxy(93, row + 10);
    
	    char s[10];
	    cin.getline(s, 10);
	
	    float d = atof(s);
	    if(d >= 0 && d <= 10){
	        list[row]->dk.DIEM = d;
	    }
	}
		
        else if(c == 27){ //ESC
        	SaveFileDSDK(ds, filenameDSDK);
            break;
        }
    }
    cout<<"\nDa luu diem!\n";
    getch();
}

//cau j
void InBangDiem(PTRSV FirstSV, DSLTC &ds, treeMH rootMH){
	char nienkhoa[10];
	if (ChonNienKhoaTheoGoiY(ds,nienkhoa)==0) return ;
	cout<<"\n";
	int hocky;
	ChonHocKyTheoGoiY( ds,nienkhoa,hocky);
	cout<<"\n";
    for (int i=0; i<ds.n; i++){ //goi nho
    	if (strcmp(ds.nodes[i]->NienKhoa, nienkhoa)==0 && ds.nodes[i]->Hocky==hocky)
    	{
    		treeMH mh=TimMH(rootMH, ds.nodes[i]->MAMH);
    		if (mh!=NULL){
    			cout<<"Ma mon hoc:"<<mh->mh.MAMH
    			    <<"| Ten mon hoc:"<<mh->mh.TENMH
    			    <<"| Nhom:"<<ds.nodes[i]->Nhom<<endl;
			}
		}
	}
	char mamh[10];
	if (ChonMaMonHocTheoGoiY(ds, mamh,nienkhoa,hocky)==0) return ;
	cout<<"\n";
	int nhom;
	ChonNhomTheoGoiY(ds,nienkhoa,hocky, mamh,nhom);
    
    LopTinChi* ltc = TimLTC(ds, nienkhoa, hocky, mamh, nhom);
    
    //kiem tra ltc va dssvdk rong
    if(ltc == NULL){
        cout << "Khong tim thay lop tin chi!\n";
        getch(); 
        return; 
    }
    if(ltc->dssvdk == NULL){
        cout << "Chua co sinh vien dang ky!\n";
        getch(); 
        return;
    }
    
    //In tieu de
    treeMH mh = TimMH(rootMH, mamh);
	if(mh != NULL)
	    cout << "\nBANG DIEM MON HOC: " << mh->mh.TENMH;
    cout << "\nNien khoa: " << nienkhoa;
    cout << "  Hoc ky: " << hocky;
    cout << "  Nhom: " << nhom << endl;

    cout << right
    	<< setw(7)  << "STT"
     	<< " | " << setw(16) << "MASV"
     	<< " | " << setw(25) << "HO"
     	<< " | " << setw(25) << "TEN"
     	<< " | " << setw(7)  << "DIEM"
     	<< " |\n";
     	
     	//kiem tra sv da thi hay chua 
	    PTRDK p = ltc->dssvdk;
		bool coDiem = false;	
		while(p != NULL){
		    if(!p->dk.huydangky && p->dk.DIEM >= 0){
		        coDiem = true;
		        break;
		    }
		    p = p->next;
		}
		if(!coDiem){
		    cout<<"Chua co sinh vien thi!\n";
		    getch(); 
		    return;
		}
	
	p = ltc->dssvdk;
    int stt = 1;
    while(p != NULL){
        PTRSV sv = TimSV(FirstSV, p->dk.MASV);
        if(sv != NULL && p->dk.huydangky == false && p->dk.DIEM >= 0){
            cout << right
                << setw(7) << stt 
     			<< " | " << setw(16) << sv->sv.MASV
     			<< " | " << setw(25) << sv->sv.HO
     			<< " | " << setw(25) << sv->sv.TEN
     			<< " | ";
			if(p->dk.DIEM >= 0)
    			cout << right << setw(7) << p->dk.DIEM;
			else
    			cout << setw(5) << "";
			cout << " |\n";
            stt++;
        }
        p = p->next;
    }
    getch(); 
}


//cau k
void InDiemTongKet(PTRSV FirstSV, DSLTC ds, treeMH rootMH){
Nhaplaimalop:
	PTRSV sv;
 	char malop[20];
	ChonMaLopTheoGoiY(FirstSV,  malop);
	
	if(strlen(malop)==0) return;
	if(!TonTaiLop(FirstSV,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}

    cout << "\n\n                   =====BANG THONG KE DIEM TRUNG BINH KHOA HOC=====\n";
	cout << "\tLop: " << malop << endl;
    cout << right
    	 << setw(7)  << right  << "STT"
         << " | " << setw(16) << right  << "MASV"
	     << " | " << setw(25) << right  << "HO"
	     << " | " << setw(25) << right  << "TEN"
	     << " | " << setw(7)  << right << "DTB"
	     << " |\n";
	int stt = 1;
    for(PTRSV sv = FirstSV; sv != NULL; sv = sv->next){
        if(strcmp(sv->sv.MALOP, malop) != 0) continue;

        float tong = 0;
        int tongTC = 0;
        // duyet tat ca cac lop tin chi 
        for(int i = 0; i < ds.n; i++){
            LopTinChi* ltc = ds.nodes[i];
            PTRDK p = ltc->dssvdk;
            while(p != NULL){
                if(strcmp(p->dk.MASV, sv->sv.MASV) == 0 &&
                   p->dk.huydangky == false &&
                   p->dk.DIEM >= 0){
                    treeMH mh = TimMH(rootMH, ltc->MAMH);
                    if(mh != NULL){
                        int sotc = mh->mh.STCLT + mh->mh.STCTH;
                        tong += p->dk.DIEM * sotc;
                        tongTC += sotc;
                    }
                }
                p = p->next;
            }
        }
        float dtb = (tongTC > 0) ? tong / tongTC : 0;
        cout << right
			 << setw(7)  << right  << stt
             << " | " << setw(16) << right  << sv->sv.MASV
		     << " | " << setw(25) << right  << sv->sv.HO
		     << " | " << setw(25) << right  << sv->sv.TEN
		     << " | " << setw(7)  << right << fixed << setprecision(2) << dtb
		     << " |\n";
		stt++; 
    }
    getch(); 
}


//cau l
void InBangDiemTongKet(PTRSV FirstSV, DSLTC ds, treeMH rootMH){
Nhaplaimalop:
	PTRSV sv;
 	char malop[20];
	ChonMaLopTheoGoiY(FirstSV,  malop);
	
	if(strlen(malop)==0) return;
	if(!TonTaiLop(FirstSV,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}
	
	if(strlen(malop)==0) return;
	if(!TonTaiLop(FirstSV,malop)){
		cout << "Ma lop sai. Vui long nhap lai!\n";
		goto Nhaplaimalop; 
	}
	 
    //Lay ds mon
	char dsMH[100][11]; // luu MAMH
	int nMH = 0;
	
	for(int i = 0; i < ds.n; i++){
	    LopTinChi* ltc = ds.nodes[i];
	
	    // duyet ds dang ky
	    PTRDK p = ltc->dssvdk;
	    while(p != NULL){
	        // chi lay sv thuoc lop
	        PTRSV sv = TimSV(FirstSV, p->dk.MASV);
	
	        if(sv != NULL 
	            && strcmp(sv->sv.MALOP, malop) == 0
	            && p->dk.huydangky == false
	            && p->dk.DIEM >= 0)
	        {
	            // kiem tra trung MAMH
	            bool trung = false;
	            for(int k = 0; k < nMH; k++){
	                if(strcmp(dsMH[k], ltc->MAMH) == 0){
	                    trung = true;
	                    break;
	                }
	            }
	
	            if(!trung){
	                strcpy(dsMH[nMH], ltc->MAMH);
	                nMH++;
	            }
	        }
	        p = p->next;
	    }
	}
    
    bool coSV = false;
    for(PTRSV sv = FirstSV; sv != NULL; sv = sv->next){
    	if(strcmp(sv->sv.MALOP, malop) != 0) continue;

	    coSV = true;
	}
	if(!coSV){
		cout << "Khong co sinh vien thuoc lop!\n";
		getch();
	}
	
    //Tieu de
    cout << "\n\n                                          =====BANG DIEM TONG KET=====\n";
    cout << "                                                      Lop: " << malop << endl;
    
    // Header
    int soMonMoiTrang = 4; //so mon hien thi tren moi bang

	for(int start = 0; start < nMH; start += soMonMoiTrang){
	    int end = min(start + soMonMoiTrang, nMH);
	
	    // In header
	    cout << "\n    STT|            MASV|              HO|             TEN";
	    for(int i = start; i < end; i++){
	        cout << "|" << setw(13) << dsMH[i];
	    }
	    cout << "|\n";
	
	    // In tung sinh viên
	    int stt = 1;
	    for(PTRSV sv = FirstSV; sv != NULL; sv = sv->next){
	        if(strcmp(sv->sv.MALOP, malop) != 0) continue;
	
	        cout << setw(7) << stt
	             << "|" << setw(16) << sv->sv.MASV
	             << "|" << setw(16) << sv->sv.HO
	             << "|" << setw(16) << sv->sv.TEN;
	
	        for(int i = start; i < end; i++){
	            float diem = -1;

				// duyet LTC de tim diem
				for(int j = 0; j < ds.n; j++){
				    LopTinChi* ltc = ds.nodes[j];
				
				    if(strcmp(ltc->MAMH, dsMH[i]) == 0){
				        PTRDK p = ltc->dssvdk;
				        while(p != NULL){
				            if(strcmp(p->dk.MASV, sv->sv.MASV) == 0 &&
				               p->dk.huydangky == false &&
				               p->dk.DIEM >= 0){
				                diem = p->dk.DIEM;
				                break;
				            }
				            p = p->next;
				        }
				    }
				}
	
	            if(diem >= 0)
	                cout << "|" << setw(13) << diem;
	            else
	                cout << "|" << setw(13) << "";
	        }
	        cout << "|\n";
	        stt++;
	    }
	}
	getch();
}



//int OpenFileDSDK(DSLTC &ds, char *filename){
//    FILE *f = fopen(filename, "r");
//    if(f == NULL) return 0;
//
//    int malop;
//    DangKy dk;
//    int temp; //tao 1 bien tam 
//	//vi fscanf chi doc int ko doc bool 
//
//    while(fscanf(f, "%d|%[^|]|%f|%d\n",
//        &malop,
//        dk.MASV,
//        &dk.DIEM,
//        temp) == 4){ 
//		//==4 vi co 4 bien, neu dung !=EOF se chay vo han, hoac loi khi file loi 
//        int pos = TimLTC(ds, malop);
//       if(pos != -1){
//			InsertLast_DK(ds.nodes[pos]->dssvdk, dk);
//        }
//    }
//    fclose(f);
//    return 1;
//}
int OpenFileDSDK(DSLTC &ds, char *filename){
    FILE *f = fopen(filename, "r");
    if(f == NULL) return 0;

    int malop;
    DangKy dk;

    while(fscanf(f, "%d|%[^|]|%f|%d\n",
        &malop,
        dk.MASV,
        &dk.DIEM,
        &dk.huydangky)==4){
        int pos = TimLTC(ds, malop);
        if(pos != -1){
			InsertLast_DK(ds.nodes[pos]->dssvdk, dk);
        }
    }
    fclose(f);
    return 1;
}



int main(){
 	treeMH rootMH=NULL;
    PTRSV FirstSV=NULL;	
 	DSLTC dsLTC; MonHoc mh;
 	SinhVien sv;
 	int chon;char masv[20];
 	
    OpenFileDSLTC(dsLTC, filenameDSLTC);
    OpenFileDSSV(FirstSV, filenameDSSV);
    OpenFileDSMH(rootMH, filenameDSMH);
	OpenFileDSDK(dsLTC, filenameDSDK); 
	 
 	do{
 		chon=Menu(thucdon);
 		switch(chon){
 			case 1:{
 				int chonLTC;
 					do{
                        clrscr();
                        cout << "\n===== QUAN LY LOP TIN CHI =====\n";
                        cout << "1. Them lop tin chi\n";
                        cout << "2. Hieu chinh lop tin chi\n";
                        cout << "3. Xoa lop tin chi\n";
                        cout << "4. Liet ke DSLTC\n";
                        cout << "5. Thoat\n" ;
                        cout << "Chon: ";
                        cin >> chonLTC;
                        clrscr();
        		        switch(chonLTC){
                            case 1: 
                              	clrscr(); 
                                while(ThemLTC(dsLTC,rootMH)==1) {
                                    clrscr();
                                }
                                break;
                            case 2:{
                               if ( HieuChinhLTC(dsLTC,rootMH)==0) break;
//                             
                            }
                            case 3:{
                                
                                if (!XoaLTC(dsLTC)) break;
                            }
            				case 4:{
                                LietKeDSLTC (dsLTC);
                                getch(); 
                                break;
                            }
                            case 5:{ 
                            	system ("cls");
            	        		if (SaveFileDSLTC(dsLTC, filenameDSLTC)==0) cout << "Loi mo file de ghi ";
            	        		else cout << "Da ghi xong danh sach lop tin chi vao file.";
                                getch();
                                break;
                            }
                            default:
                                cout<<"Lua chon khong hop le\n";
                        }
                    } while(chonLTC!=5);
                            break;
                }
            case 2:{
                system ("cls");
                if (InSVdkLTC( dsLTC, FirstSV, rootMH)==0) break;
                getch();
                break;
            }
            case 3:{
                clrscr();
                int chonLTC;
                do{
                    clrscr();
                    cout << "\n===== QUAN LY LOP SINH VIEN =====\n";
                    cout << "1. Them sinh vien vao lop \n";
                    cout << "2. Hieu chinh thong tin sinh vien\n";
                    cout << "3. Xoa sinh vien\n";
    				cout << "4. In DSSV 1 lop tang dan theo ma sinh vien\n";
                    cout << "5. Thoat\n";
                    cout << "Chon: ";
                    cin >> chonLTC;
                    clrscr();
                    switch(chonLTC){
                        case 1:{ 
						if (!NhapSV(FirstSV,sv)) break;

						
						else 
						{ getch();
						break;
						}
                          
                        }
                        case 2:{
                            if (!HieuChinhSV( FirstSV))
                            break;
                        }
                        case 3:{	
                            if(!XoaMaSV(FirstSV)) break;
                        }
            			case 4:{
            				system ("cls");
                            InDSSV_1lop_TheoMa(FirstSV);
                            getch();
                            break;
                        }
                        case 5:{
                        	system ("cls");
            	        	if (SaveFileDSSV(FirstSV, filenameDSSV)==0) cout << "Loi mo file de ghi ";
            	        	else cout << "Da ghi xong danh sach sinh vien vao file.";
            	       		getch();
							break;
							}
                        default:
                            cout<<"Lua chon khong hop le\n";
                        }
                    } while(chonLTC!=5);
                        break;
                    }
		    case 4:{ 
                clrscr();
                InDSSV_1lop_TheoHo_Ten(FirstSV);
                getch();
                break;
            }
            case 5:{
                clrscr();
                int chonLTC;
                do{
                clrscr();
                cout << "\n===== QUAN LY MON HOC =====\n";
                cout << "1. Them mon hoc \n";
                cout << "2. Hieu chinh thong tin mon hoc\n";
                cout << "3. Xoa mon hoc\n";
                cout << "4. Thoat\n";
                cout << "Chon: ";
                cin >> chonLTC;
                clrscr();
                switch(chonLTC){
                    case 1:{ 
                        while(NhapMH(rootMH));
                        break;
                    }
                    case 2:{
                        if (!HieuChinhMH(rootMH)) break;
                        
                    }
                    
					case 3: {	
   						Nhaplaimamonhoc:
   						cout<<"XOA MON HOC";
   						cout<<"\n";

    					char c [15] ; 
    					ChonMaMH_TheoGoiY( rootMH,  c);
						if (strcmp(c, "0") == 0) {
        				delete[] c; 
       					break; 
    					}
   						if (XoaMH(rootMH, c) == 1) {
        				cout << "=> Xoa mon hoc thanh cong!\n";
    						} 
   						else {
        				cout << "=> Loi: Khong tim thay ma mon hoc phu hop!\n";
   						 }

    					delete[] c;
    					goto Nhaplaimamonhoc; 
    					break;
						}
                    case 4: {
						system ("cls");
                        if (SaveFileDSMH(rootMH, filenameDSMH)==0) cout << "Loi mo file de ghi ";
                        else cout << "Da ghi xong danh sach mon hoc vao file.";
                       	getch();
                        break;
                    }
						
                    default:
                        cout << "Lua chon khong hop le\n";
                    }		 
			    } while(chonLTC!=4);
			    break;
            }
			case 6:{ 	
                system ("cls");
			    InDSMH_TangTheoTen(rootMH);
			    getch();
				break;
			}
			case 7:{	
                clrscr();
                if(DangKyLopTinChi(dsLTC,FirstSV,rootMH)==0)
				break;
			}
			case 8:{
				clrscr();
                char nienkhoa[10];
                int hocky;
                cout <<"Nhap nien khoa muon kiem tra huy:";
                cin >> nienkhoa;
                cout <<"Nhap hoc ky muon kiem tra huy:";
                cin >> hocky;
                HuyLTC(dsLTC,nienkhoa,hocky);
                getch();
				break;
			}
			case 9:{
				clrscr();
				cout << "Nhap va sua diem vao danh sach:\n"; 
	   			NhapSuaDiem(dsLTC, FirstSV, rootMH);
    			break;
			}
			case 10:{
				clrscr();
				cout << "Nhap thong tin de in bang diem:" << endl;
				InBangDiem(FirstSV, dsLTC, rootMH);
				break;
			}
			case 11:{
				clrscr();
				InDiemTongKet(FirstSV, dsLTC, rootMH);
				break;
			}
			case 12:{
				clrscr();
				InBangDiemTongKet(FirstSV, dsLTC, rootMH);
				break;
			}
			case 13:{
                SaveFileDSSV(FirstSV, filenameDSSV);
                SaveFileDSMH(rootMH, filenameDSMH);
                SaveFileDSDK(dsLTC, filenameDSDK);
				SaveFileDSLTC(dsLTC, filenameDSLTC); //quan trong

                cout<<"Da luu du lieu. Thoat chuong trinh!\n";
                getch();
			    return 0;
            }
		}
	} while(1);
	
	return 0 ;
}
 	
 	
 	
	

 	
 

