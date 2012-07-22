#include "Form1.h"
#include <windows.h>

using namespace PriceChecker;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;

#pragma comment(lib,"User32.lib")

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Включение визуальных эффектов Windows XP до создания каких-либо элементов управления
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Создание главного окна и его запуск
	Application::Run(gcnew Form1());
	return 0;
}

Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	//label
	old_price_label->Visible = false;
	action_label->Visible = false;
	balance_label->Visible = false;

	//para
	old_price_para->Visible = false;
	balance_para->Visible = false;
	price_para->Visible = false;

	//full screen size after form load
	Form1::Size = System::Drawing::Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Form1::CenterToScreen();

	timer1->Enabled = true;

}

Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	Random^ rnd=gcnew Random();
	int i=1+rnd->Next(10);

	String^ path= String::Format("{0}\\image\\{1}.jpg",Application::StartupPath,i);
	pictureBox1->ImageLocation = path;
	pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;
	
}

Void Form1::barcode_text_box_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	int len = barcode_text_box->TextLength ;
	int barcode[13];
	
	if (len == 13)
	{
		for (int i=0; i< len; i++)
		{
			barcode[i] = this->barcode_text_box->Text[i];
		}
		ean13_validate(barcode);
	}
}

Boolean Form1::ean13_validate(int barcode[])
{	
	Boolean check = false;
	Int32 summ = ((barcode[1]- '0')+(barcode[3]- '0')+(barcode[5]- '0')+(barcode[7]- '0')+(barcode[9]- '0')+(barcode[11]- '0'))*3+((barcode[0]- '0')+(barcode[2]- '0')+(barcode[4]- '0')+(barcode[6]- '0')+(barcode[8]- '0')+(barcode[10]- '0'));
	Int32 ten = summ/10;
	summ = 10-(summ - (ten*10));
	if(summ == (barcode[12]- '0'))
	    check = true;
	else
	    check = false;
    return check;
}