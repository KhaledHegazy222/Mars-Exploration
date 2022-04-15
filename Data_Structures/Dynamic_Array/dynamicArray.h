#ifndef DYNAMIC_ARRAY_H
	#define DYNAMIC_ARRAY_H



	
	template <typename var> 
	class dynamicArray
	{
		private:
			var* vec;
			int capacity,currentSize;

			void updateSize();

		public:




			// Constructors & Destructor
			dynamicArray();
			dynamicArray(int);
			dynamicArray(int,var);
			dynamicArray(const dynamicArray<var>&);
			~dynamicArray();



			// Public Methods
			var& front();
			var& back();
			void pushBack(var);
			void popBack();
			int size();
			void clear();
			

			// Operators
			var& operator[] (int);
			dynamicArray<var>& operator=(dynamicArray<var>&);

	};

	



	#include "dynamicArray.cpp"
	
#endif
