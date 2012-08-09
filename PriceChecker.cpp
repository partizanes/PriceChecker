#include "Form1.h"
#include <windows.h>

bool pass = false;

using namespace PriceChecker;
using namespace System::IO;
using namespace System::Net;
using namespace System::Data;
using namespace System::Net::Sockets;
using namespace System::Globalization;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;
using namespace System::Runtime::Serialization::Formatters::Binary;


#pragma comment(lib,"User32.lib")

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	if (System::Diagnostics::Process::GetProcessesByName(Application::ProductName)->Length > 1)
	{
		MessageBox::Show("Приложение уже запущено!");
		return 0;
	}
	else
	{
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);

		Application::Run(gcnew Form1());
		return 0;
	}
}

Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	//label
	old_price_label->Visible = false;
	action_label->Visible = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	weight_label->Visible = false;
	total_label->Visible = false;

	//para
	old_price_para->Visible = false;
	price_para->Visible = false;
	weight_para->Visible = false;
	total_para->Visible = false;

	//full screen size after form load
	Form1::Size = System::Drawing::Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Form1::CenterToScreen();

	//lineshape
	lineShape1->Visible = false;
	lineShape2->Visible = false;

	//testing system
	char buf[6];

	GetPrivateProfileString("SETTINGS", "start_check","true",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	if(buf == "true")
	{
		diag_system();
	}

	//timer for random image
	timer1->Enabled = true;

	log_write("Терминал запущен!","SYSTEM","pc");

	//logo after load
	String^ path= String::Format("{0}\\image\\logo.jpg",Application::StartupPath);
	pictureBox1->ImageLocation = path;
	pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;

	//set upload_log_interval from config default 3hours
	log_upload_timer->Interval = (GetPrivateProfileInt("SETTINGS", "upload_log_interval",3,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini")))*3600000;
	log_upload_timer->Enabled = true;
}

Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	Random^ rnd=gcnew Random();
	int i=1+rnd->Next(GetPrivateProfileInt("SETTINGS", "last_image_num",1,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini")));

	String^ path= String::Format("{0}\\image\\{1}.jpg",Application::StartupPath,i);

	if(!File::Exists(path))
	{

		String^ path= String::Format("{0}\\image\\logo.jpg",Application::StartupPath);
		pictureBox1->ImageLocation = path;
		pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;

		log_write("Изображение не найдено: "+path,"SYSTEM","pc");
		return;
	}

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

Void Form1::log_write(String^ str,String^ reason,String^ logname)
{
	String^ EntryTime = (gcnew DateTime())->Now.ToLongTimeString();
	String^ EntryDate = (gcnew DateTime())->Today.ToShortDateString();
	if(!Directory::Exists(Environment::CurrentDirectory+"/log/"))
	{
		Directory::CreateDirectory((Environment::CurrentDirectory+"/log/"));
	}
	String^ fileName = "log/"+logname+".log";
	StreamWriter^ sw = gcnew StreamWriter(fileName,true,System::Text::Encoding::UTF8);
	sw->WriteLine("["+EntryDate+"]["+EntryTime+"]["+reason+"]"+" "+str);
	sw->Close();
}

Void Form1::query(String^ bar)
{
	char buf[50];

	GetPrivateProfileString("SETTINGS", "srv_global","192.168.1.100",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
        CharToSystemString(buf), "admin", "12345", "ukmserver");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		char buf[14];
		GetPrivateProfileString("SETTINGS", "id_pricelist","1",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

		cmd = gcnew MySqlCommand("SELECT a.name, b.price \n"
			 "FROM trm_in_var C \n"
			 "LEFT JOIN trm_in_items A ON A.id=C.item \n"
			 "LEFT JOIN trm_in_pricelist_items B ON B.item=c.item \n"
			 "WHERE C.id='"+bar+"' \n"
			 "AND (b.pricelist_id="+CharToSystemString(buf)+")", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			price_para->Visible = true;
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =Convert::ToString(reader->GetInt32(1));
			barcode_text_box->Text = "";
			msg_label->Text = "";
			panel4->Visible = false;

			action_check(bar);
		}
		else
		{
			log_write("Не найден "+bar,"SYSTEM","pc");
			barcode_text_box->Text = "";
			set_msg_on_timer("Штрих-код не найден!Обратитесь к продавцу!");
		}

	}
	catch (Exception^ exc)
	{
		log_write(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}
}

Void Form1::set_msg_on_timer(String^ text)
{
	panel4->Visible = true;
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

		old_price_label->Visible = false;
		old_price_para->Visible = false;
		action_visible->Enabled = false;
		action_label->Visible = false;

		old_price_para->Text = "";
		lineShape1->Visible = false;
		lineShape2->Visible = false;

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
				set_msg_on_timer("Тестирование системы");
				stg_panel->Visible = true;
				barcode_text_box->Text = "";
			}
			else
			{
				barcode_text_box->Text = "";
				set_msg_on_timer("Данные не верны!");
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
		case 5:
			//короткие коды на весовые товары(+прочие) ,цена за килограмм(цена за штуку)
			//практическое применение:сверка продавцами ценников и цен на кассе с помощью прайсчекера.
			//вынужденое использование отдельного запроса
			//тестовая функция

			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			for (int i=0; i< len; i++)
			{
				bar += this->barcode_text_box->Text[i];
			}

			queryfive(bar);
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

				log_write(bar,"NOTVALID[EAN8]","nv");

				    break;
			}
		case 10:
			if(!pass)
			{
				set_msg_on_timer("Авторизация не пройдена!");
				log_write("false","PASS","auth");
				barcode_text_box->Text = "";
				break;
			}
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 9 && (barcode[1]- '0') == 9 && (barcode[2]- '0') == 9 && (barcode[3]- '0') == 9 && (barcode[4]- '0') == 9 && (barcode[5]- '0') == 9 && (barcode[6]- '0') == 9 && (barcode[7]- '0') == 9 && (barcode[8]- '0') == 9  && (barcode[9]- '0') == 9) 
			{
				set_msg_on_timer("                                       Настройки");
				barcode_text_box->Text = "";
				stg_panel->Visible = true;
				log_write("true","PASS","auth");
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

				if (Convert::ToInt32(weight) == 0)
				{
					barcode_text_box->Text = "";
					set_msg_on_timer ("Вес товара не может быть равен нулю!") ;
					log_write(weight,"WEIGHT","pc");
					return;
				}


				query(bar);

				if(price_para->Text == "Price")
				{
					set_msg_on_timer ("Товар не найден!!!") ;
					log_write("Ошибка!Товар не прогружен!","NOTFOUND","ERROR");
					return;
				}

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
				log_write(bar,"NOTVALID[EAN13]","nv");
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
			//TODO Предложить изменить настройки сервера.
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
	char buf[50];

	GetPrivateProfileString("SETTINGS", "srv_global","192.168.1.100",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "admin", "12345", "ukmserver");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();
		return true;
	}
	catch (Exception^ exc)
	{
		log_write(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
		return false;
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}

}

Void Form1::close_menu_Click(System::Object^  sender, System::EventArgs^  e)
{
	stg_panel->Visible = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	msg_label->Text = "";

}

Void Form1::test_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	dir_exist_para->Text = "Проверка...";
	mysql_check_para->Text = "Проверка...";

	diag_system();

	stg_panel->Visible = true;
	panel4->Visible = true;
	msg_label->Visible = true;

	msg_label->Text = "                       Тестирование системы";
}

Void Form1::action_check(String^ bar)
{
	char buf[50];

	GetPrivateProfileString("SETTINGS", "srv_local","192.168.1.11",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "pricechecker", "7194622Parti", "action");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		CultureInfo^ jaJP = gcnew CultureInfo("ja-JP");

		String^ EntryDate =  (gcnew DateTime())->Today.ToString("d",jaJP)->Replace("/","-");

		cmd = gcnew MySqlCommand("SELECT price_old,price_new FROM action_price WHERE barcode = "+bar+" AND price_new = "+price_para->Text+" AND start_action <= '"+EntryDate+"' AND stop_action >= '"+EntryDate+"'", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			action_label->Visible = true;
			action_visible->Enabled = true;
			old_price_label->Visible = true;
			lineShape1->Visible = true;
			lineShape2->Visible = true;
			old_price_para->Visible = true;
			old_price_para->Text = reader->GetString(0);
		}
	}
	catch (Exception^ exc)
	{
		log_write(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}
}

char* Form1::SystemStringToChar(System::String^ string)
{
	return (char*)(void*)Marshal::StringToHGlobalAnsi(string);
}

String^ Form1::CharToSystemString(char* ch)
{
	char * chr=new char[]=ch;
	System::String^ str;
	for(int i=0;chr[i]!='\0';i++)
	{
		str+=wchar_t(ch[i]);
	}
	return str;
}

Void Form1::opt_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	char buf[50];

	GetPrivateProfileString("SETTINGS", "srv_local","192.168.1.11",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "pricechecker", "7194622Parti", "action");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		cmd = gcnew MySqlCommand("OPTIMIZE TABLE action_price", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			set_msg_on_timer(reader->GetString(3));
		}
		else
		{
			set_msg_on_timer("                       Ошибка!");
		}
		while(reader->Read())
		{
			set_msg_on_timer("                       "+reader->GetString(3));
		}

	}
	catch (Exception^ exc)
	{
		log_write(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}
}

Void Form1::upload_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	upload_button->Enabled = false;
	backgroundWorker1->RunWorkerAsync();
}

Void Form1::backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	String^ file_name;
	Boolean okey;

	for (int i=1; i< 4; i++)
	{
		switch (i)
		{
		case 1:
			file_name = "pc.log";
			break;
		case 2:
			file_name = "nv.log";
			break;
		case 3:
			file_name = "auth.log";
			break;
		}

		 try
		 {
			 FileStream^ fs = File::OpenRead( Environment::CurrentDirectory+"/log/"+file_name );
			 array<Byte>^ buffer = gcnew array<Byte>(fs->Length);
			 int len = safe_cast<int>(fs->Length);
			 fs->Read(buffer,0,len);
			 fs->Close();

			 char buf[17];

			 GetPrivateProfileString("SETTINGS", "upload_server","error",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

			 if(buf == "error")
			 {
				 set_msg_on_timer("Не задан адрес сервера в файле конфигурации!");
				 return;
			 }

			 BinaryFormatter ^br = gcnew BinaryFormatter ();
			 TcpClient ^myclient = gcnew TcpClient (CharToSystemString(buf),7000);
			 NetworkStream ^myns = myclient->GetStream ();
			 br->Serialize (myns,file_name);

			 BinaryWriter ^mysw = gcnew BinaryWriter (myns);
			 mysw->Write(buffer);
			 mysw->Close();

			 myns->Close();
			 myclient->Close();

			 Sleep(1000);
		 }
		 catch (Exception^ exc)
		 {
			 okey = false;
			 log_write(exc->Message,"EXCEPTION","pc");
		 }
		 finally
		 {
			 if(okey)
				 log_write("Логи выгружены успешно!","SYSTEM","pc");
		 }
	}
}

Void Form1::backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e)
{
	upload_button->Enabled = true;
}

Void Form1::log_upload_timer_Tick(System::Object^  sender, System::EventArgs^  e)
{
	log_write("Автоматическая выгрузка логов","SYSTEM","pc");
	backgroundWorker1->RunWorkerAsync();
}

Void Form1::queryfive(String^ bar)
{
	char buf[50];

	GetPrivateProfileString("SETTINGS", "srv_global","192.168.1.100",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	String^ connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "admin", "12345", "ukmserver");

	conn = gcnew MySqlConnection(connStr);

	MySqlDataReader^ reader = nullptr;

	try
	{
		conn->Open();

		char buf[4];
		GetPrivateProfileString("SETTINGS", "id_pricelist","1",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

		cmd = gcnew MySqlCommand("SELECT a.name, b.price \n"
			"FROM trm_in_var C \n"
			"LEFT JOIN trm_in_items A ON A.id=C.item \n"
			"LEFT JOIN trm_in_pricelist_items B ON B.item=c.item \n"
			"WHERE a.id='"+bar+"' \n"
			"AND (b.pricelist_id="+CharToSystemString(buf)+")", conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			price_para->Visible = true;
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =Convert::ToString(reader->GetInt32(1));
			barcode_text_box->Text = "";
			msg_label->Text = "";
			panel4->Visible = false;

			action_check(bar);
		}
		else
		{
			barcode_text_box->Text = "";
			set_msg_on_timer("Штрих-код не найден!Обратитесь к продавцу!");
		}

	}
	catch (Exception^ exc)
	{
		log_write(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();
	}
		 }