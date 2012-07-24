﻿#include "Form1.h"
#include <windows.h>

using namespace PriceChecker;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;
using namespace System::IO;

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
	msg_label->Visible = false;

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
	Int32 barcode[13];

	switch (len)
	{
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
		break;
	case 13:

		for (int i=0; i< len; i++)
		{
			barcode[i] = this->barcode_text_box->Text[i];
		}

		if(ean13_validate(barcode))
		{
			String^ bar;

			for (int i=0; i< len; i++)
			{
				bar += this->barcode_text_box->Text[i];
			}

			query(bar);
		}
		else
		{
			msg_label->Visible = true;
			msg_label->Text = "Возможно,штрих-код был считан неверно,попробуйте еще раз";
			msg_clear->Enabled = true;

			String^ bar;

			for (int i=0; i< len; i++)
			{
				bar += this->barcode_text_box->Text[i];
			}

		    log_write(bar,"NOTVALID[EAN13]");
		}
		break;
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

void Form1::log_write(String^ str,String^ reason)
{
	String^ EntryTime = (gcnew DateTime())->Now.ToLongTimeString();
	String^ EntryDate = (gcnew DateTime())->Today.ToShortDateString();
	String^ fileName = "PriceChecker.log";
	StreamWriter^ sw = gcnew StreamWriter(fileName,true,System::Text::Encoding::ASCII);
	sw->WriteLine("["+EntryDate+"]["+EntryTime+"]["+reason+"]"+" "+str);
	sw->Close();
}

Void Form1::query(String^ bar)
{
	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		"192.168.1.3", "root", "7194622Parti", "ukmserver");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		cmd = gcnew MySqlCommand("SELECT a.name, b.price FROM trm_in_var C LEFT JOIN trm_in_items A ON A.id=C.item LEFT JOIN (SELECT * FROM trm_in_pricelist_items WHERE pricelist_id=1 OR pricelist_id=6) B ON B.item=c.item WHERE C.id='"+bar+"'", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(!reader->Read())
		{
			msg_label->Visible = true;
			msg_label->Text = "Штрих-код не найден,обратитесь к продавцу!";
			msg_clear->Enabled = true;
		}

		while(reader->Read())
		{
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =reader->GetString(1);
		}

	}
	catch (Exception^ exc)
	{
		msg_label->Visible = true;
		msg_label->Text = ("Exception: " + exc->Message);
		msg_clear->Enabled = true;
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}
}