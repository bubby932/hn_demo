#pragma once

namespace must {
	enum ErrorKind
	{
		Other,
		OutOfMemory,
		InvalidArgument,
		InvalidOperation,
	};

	class Error
	{
		public:

		private:
		ErrorKind kind;
		
	};
}