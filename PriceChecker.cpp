#include "Form1.h"
#include <windows.h>

bool pass = false;

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
	panel4->Visible = false;
	weight_label->Visible = false;
	total_label->Visible = false;

	//para
	old_price_para->Visible = false;
	balance_para->Visible = false;
	price_para->Visible = false;
	weight_para->Visible = false;
	total_para->Visible = false;

	panel4->Visible = false;

	//full screen size after form load
	Form1::Size = System::Drawing::Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Form1::CenterToScreen();

	//testing system
	diag_system();

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

Boolean Form1::ean8_validate(int barcode[])
{
	Boolean check = false;

	Int32 summ = (barcode[0]- '0')*3+(barcode[1]- '0')+(barcode[2]- '0')*3+(barcode[3]- '0')+(barcode[4]- '0')*3+(barcode[5]- '0')+(barcode[6]- '0')*3+(barcode[7]- '0');
	Int32 ten = summ/10;


	if(summ - (ten*10) == 0)
		check = true;
	else
		check = false;

	return check;
}

Void Form1::log_write(String^ str,String^ reason)
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
		"192.168.1.100", "admin", "12345", "ukmserver");
	    /*"192.168.1.3", "root", "7194622Parti", "ukmserver");*/

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		cmd = gcnew MySqlCommand("SELECT a.name, b.price FROM trm_in_var C LEFT JOIN trm_in_items A ON A.id=C.item LEFT JOIN (SELECT * FROM trm_in_pricelist_items WHERE pricelist_id=1 OR pricelist_id=6) B ON B.item=c.item WHERE C.id='"+bar+"'", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			price_para->Visible = true;
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =Convert::ToString(reader->GetInt32(1));
			barcode_text_box->Text = "";
			msg_label->Text = "";
			panel4->Visible = false;
		}
		else
		{
			set_msg_on_timer("Штрих-код не найден!Обратитесь к продавцу!");
		}

	}
	catch (Exception^ exc)
	{
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
		msg_clear->Enabled = true;
	}
}

Void Form1::set_msg_on_timer(String^ text)
{
	msg_clear->Enabled = true;
	msg_label->Visible = true;
	msg_label->Text = text;
}

Void Form1::barcode_text_box_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{

	if (e->KeyCode == Keys::Enter)
	{
		int len = barcode_text_box->TextLength ;
		Int32 barcode[13];

		String^ bar;
		String^ weight;

		switch (len)
		{
		case 0:
			break;
		case 1:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}
			if((barcode[0]- '0') == 1)
			{
				set_msg_on_timer("Тестирование системы!");
				stg_panel->Visible = true;
			}
			break;
		case 2:
		case 3:
			break;
		case 4:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 9 && (barcode[1]- '0') == 9 && (barcode[2]- '0') == 9 && (barcode[3]- '0') == 9)
			{
				pass = true;
				msg_label->Visible = true;
				panel4->Visible = true;
				msg_label->Text = "Проверка Пароля...";
				barcode_text_box->Text = "";
				barcode_text_box->UseSystemPasswordChar = true ;
				pass_timer->Enabled = true;
			}

			if((barcode[0]- '0') == 0 && (barcode[1]- '0') == 0 && (barcode[2]- '0') == 0 && (barcode[3]- '0') == 0)
			{
				Application::Exit();
			}

			break;
		case 8:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}
			if(ean8_validate(barcode))
			{
				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}
				query(bar);
				    break;
			}
			else
			{
				set_msg_on_timer("Возможно,штрих-код был считан неверно,попробуем найти...");

				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);

				msg_label->Text = "";

				log_write(bar,"NOTVALID[EAN8]");

				    break;
			}
		case 10:
			if(!pass)
			{
				set_msg_on_timer("Авторизация не пройдена!");
				barcode_text_box->Text = "";
				break;
			}
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 9 && (barcode[1]- '0') == 9 && (barcode[2]- '0') == 9 && (barcode[3]- '0') == 9 && (barcode[4]- '0') == 9 && (barcode[5]- '0') == 9 && (barcode[6]- '0') == 9 && (barcode[7]- '0') == 9 && (barcode[8]- '0') == 9  && (barcode[9]- '0') == 9) 
			{
				set_msg_on_timer("root@localhost:");
				break;
			}
			else
			{
				pass = false;
				pass_timer->Enabled = false;
				set_msg_on_timer("Неправильный пароль!");
				barcode_text_box->UseSystemPasswordChar = false ;
				break;
			}

		case 13:

			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 2 && (barcode[1]- '0') == 0)
			{
				for (int i=0; i< 7; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				for (int i=7; i< 12; i++)
				{
					weight += this->barcode_text_box->Text[i];
				}

				query(bar);

				float w = (float(Convert::ToInt32(weight))/1000);

				weight_para->Text = Convert::ToString(w);

				w = float((Convert::ToInt32(price_para->Text))*(w));

				weight_para->Visible = true;
				weight_label->Visible = true;
				total_para->Visible = true;
				total_label->Visible = true;
				total_para->Text = Convert::ToString(Convert::ToInt32(w));
				weight_clr->Enabled = true;
				set_msg_on_timer("Цена указана за килограмм,цена вашей покупки составит "+ Convert::ToString(Convert::ToInt32(w)));

				    break;
			}
			if(ean13_validate(barcode))
			{
				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);
			}
			else
			{
				set_msg_on_timer("Возможно,штрих-код был считан неверно,попробуем найти...");

				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);
				log_write(bar,"NOTVALID");
			}
			break;

		}
	}
}

Void Form1::pass_timer_Tick(System::Object^  sender, System::EventArgs^  e)
{
	pass = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	msg_label->Text = "";
	barcode_text_box->UseSystemPasswordChar = false ;
	pass_timer->Enabled = false;
}

Void Form1::diag_system()
{
	boolean off = false;

	stg_panel->Visible = true;
	panel4->Visible = true;
	msg_label->Visible = true;
	msg_label->Text = "Диагностика...";
	dir_exist_para->Text = "Проверка...";
	mysql_check_para->Text = "Проверка...";

	if(Directory::Exists(Environment::CurrentDirectory+"/image/"))
	{
		dir_exist_para->Text = "Доступно!";
		dir_exist_para->ForeColor = Color::Green ;
	}

	if(!Directory::Exists(Environment::CurrentDirectory+"/image/"))
	{
		dir_exist_para->ForeColor = Color::Red ;
		dir_exist_para->Text = "Не доступно!";

		String^ message = "Папка с рекламой не обнаружена,проверить еще раз?";
		String^ caption = "Ошибка";

		MessageBoxButtons buttons = MessageBoxButtons::YesNo;
		System::Windows::Forms::DialogResult result;

		result = MessageBox::Show( this, message, caption, buttons );

		if ( result == ::DialogResult::Yes )
		{
			diag_system();
		}
		else
		{
			//TODO Не закрывать программу при запуске,а генерить изображение по умолчанию.
			Application::Exit();
		}
	}

	if(mysqlcheck())
	{
		mysql_check_para->Text = "Доступно!";
		mysql_check_para->ForeColor = Color::Green ;

		off = true;
	}
	if(!mysqlcheck())
	{
		mysql_check_para->ForeColor = Color::Red ;
		mysql_check_para->Text = "Не доступно!";

		String^ message = "Не удалось соединиться с Mysql сервером,попробывать еще раз?";
		String^ caption = "Ошибка";

		MessageBoxButtons buttons = MessageBoxButtons::YesNo;
		System::Windows::Forms::DialogResult result;

		result = MessageBox::Show( this, message, caption, buttons );

		if ( result == ::DialogResult::Yes )
		{
			mysql_check_para->Text = "Проверка...";
			diag_system();
		}
		else
		{
			//TODO Не закрывать программу при запуске,а генерить изображение по умолчанию.
			Application::Exit();
		}
	}
	if(off)
	{
		panel4->Visible = false;
		msg_label->Visible = false;
		msg_label->Text = "";
		stg_panel->Visible = false;
	}
}


Boolean Form1::mysqlcheck()
{
	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
			"192.168.1.100", "admin", "12345", "ukmserver");
		/*"192.168.1.3", "root", "7194622Parti", "ukmserver");*/

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();
		return true;
	}
	catch (Exception^ exc)
	{
		set_msg_on_timer("Exception: " + exc->Message);
		return false;
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}

}