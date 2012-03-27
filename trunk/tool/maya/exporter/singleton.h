#pragma	   once
#define assert(x)


template <typename T> class Singleton
{
protected:
	static T* ms_Singleton;

public:
	Singleton(){
		assert( !ms_Singleton );
		ms_Singleton = static_cast< T* >( this );
	}
	~Singleton(){
		assert( ms_Singleton );
		ms_Singleton = 0;  
	}
	static T& getSingleton(){
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}
	static T* getSingletonPtr(){ 
		return ms_Singleton; 
	}
};