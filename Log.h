namespace Log
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Windows::Forms;


	class Write
	{
	public:
		Write();

	protected:
		~Write();

	public: static void logWrite(String^ str,String^ reason,String^ logname);

	};
}