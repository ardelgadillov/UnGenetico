#ifndef __ARREGLOS_H
#define __ARREGLOS_H

#include <string.h>

//!Clase gen�rica que almacena un arreglo de apuntadores a la clase T.
/*!El acceso a los objetos del arreglo es al estilo de los arreglos de C 
y no al estilo de listas encadenadas, lo que le da mayor rapidez.
No debe confundirse la capacidad del arreglo con la cantidad de �tems 
almacenados.
En lo posible, para evitar reasignaci�n innecesaria de memoria, antes de 
adicionar �tems al arreglo debe asignarse la capacidad necesaria para 
el n�mero de �tems que se prevee se van a adicionar al arreglo. Esto puede
hacerse mediante la funci�n \a asignarMemoria.
*/
template<class T>
class Arreglo
{
public:

	//!Constructor por copia de otro arreglo. No se puede usar con arreglos de clases abstractas
	Arreglo(const Arreglo& origen);

	//!Constructor por defecto.
	/*!Crea un arreglo vacio. No se asigna memoria al arreglo*/
	Arreglo():m_items(0),m_capacidad(0),m_pData(NULL){}

	//!Operador de asignaci�n a partir de otro arreglo. No se puede usar con arreglos de clases abstractas
	const Arreglo<T>& operator = ( const Arreglo<T>& otro );

	//!Destructor.
	/*!Elimina todos los apuntadores y destruye los objetos pertenecientes al arreglo*/
	virtual ~Arreglo(void)
	{
		Truncar(0, true, false);	//no liberamos memoria todavia
		delete [] m_pData;
	}

	//!Adiciona un apuntador al final del arreglo y retorna la posici�n en la que fue adicionado
	/*!Asigna memoria al arreglo si es necesario.
	\param Apuntador al nuevo miembro del arreglo.
	\return Posici�n en la que se adicion� el nuevo miembro.*/
	int Adicionar(T* pNuevo)
	{
		if(m_items >= m_capacidad) asignarMemoria(m_items+20);
		m_pData[m_items] = pNuevo;		
		return ++m_items;
	}

    //!Inserta un apuntador en una posici�n determinada del arreglo	
    int Insertar(T* pNuevo, int pos);

	//!Remplaza el apuntador ubicado en una posici�n determinada del arreglo por otro apuntador
	/*! 
	\param pNuevo Apuntador que har� parte del arreglo, remplazando al apuntador ubicado en la posici�n especificada.
	\param pos Posici�n del apuntador a remplazar.
	\return Apuntador reemplazado. NULL si la posicion es inv�lida*/
	T* remplazar(T* pNuevo, int pos)
	{
		if(m_items==0 || pos<0 || pos>=m_items) return NULL;
		T* temp = m_pData[pos];
		m_pData[pos] = pNuevo;		
		return temp;
	}

	//!Intercambia los apuntadores de dos posiciones determinadas
	/*!El intercambio solo es realizado si ambas posiciones son v�lidas en el arreglo.
	\param pos1 Posici�n en la que ser� ubicado el apuntador que se encuentra en la posici�n \a pos2.
	\param pos2 Posici�n en la que ser� ubicado el apuntador que se encuentra en la posici�n \a pos1. 
	\return true si el intercambio es exitoso. false si no se realiz� el intercambio.*/
	bool IntercambiarPos(int pos1, int pos2)
	{
		if(m_items<2 || pos1<0 || pos2<0 || pos1>=m_items || pos2>=m_items) return false;
		T* temp = m_pData[pos1];
		m_pData[pos1] = m_pData[pos2];
		m_pData[pos2] = temp;
		return true;
	}

	//!Reduce el tama�o del arreglo
    int Truncar(int nuevoTam, bool eliminarObjetos=true, bool liberar_memoria=true);

	//!Retorna el n�mero de �tems presentes en el arreglo
	/*!\return Tama�o del arreglo*/
    int getSize() const 
	{
		return m_items;
	}

	//!Retorna el apuntador ubicado en una posici�n determinada del arreglo
	/*!
	\param pos Posici�n del apuntador requerido. Si es menor que cero se retorna el apuntador en la primera posici�n. Si es mayor que el n�mero de �tems del arreglo, se retorna el apuntador en la �ltima posici�n.
	\return Apuntador ubicado en la posici�n \a pos.*/
	T* getPtr(int pos) const
	{
		if (pos <0) pos=0;
		else if (pos >= m_items) pos=m_items-1;
		return (m_pData[pos]);
	}

	//!Retorna una referencia al objeto ubicado en una posici�n determinada del arreglo.
	/*!Equivalente al operador [].
	\param pos Posici�n del objeto requerido. Si es menor que cero se retorna el objeto en la primera posici�n. Si es mayor que el n�mero de �tems del arreglo, se retorna el objeto en la �ltima posici�n.
	\return Referencia al objeto apuntado por el apuntador ubicado en la posici�n \a pos del arreglo*/
	T& getObj(int pos) const
	{
		if (pos <0) pos=0;
		else if (pos >= m_items) pos=m_items-1;
		return (*m_pData[pos]);
	}

	//!Retorna una referencia al objeto ubicado en una posici�n determinada del arreglo.
	/*!Es equivalente a getObj(pos). Permite acceder a los miembros del arreglo al estilo del lenguaje C.
	\param pos Posici�n del objeto requerido. Si es menor que cero se retorna el objeto en la primera posici�n. Si es mayor que el n�mero de �tems del arreglo, se retorna el objeto en la �ltima posici�n.
	\return Referencia al objeto apuntado por el apuntador ubicado en la posici�n \a pos del arreglo*/
	T& operator[]( int pos ) const 
	{
		return getObj(pos); 
	}

	//!Aumenta la capacidad del arreglo para albergar un n�mero determinado de �tems
	bool asignarMemoria(int n);

	//!Reduce la capacidad del arreglo al n�mero de �tems actual
	void liberarMemoria();

    //!Excluye del arreglo el apuntador ubicado en una posici�n determinada, sin destruir el objeto al que apunta.
    T* Detach(int pos);

    //!Excluye del arreglo el apuntador ubicado en una posici�n determinada, destruyendo el objeto al que apunta.
    void Destroy(int pos);

    //!Excluye del arreglo todos sus miembros sin destruir los objetos
	/*!Libera la memoria ocupada por el arreglo de apuntadores.*/
    void FlushDetach()
	{
		Truncar(0, false, true);		
	}

    //!Excluye del arreglo todos sus miembros y destruye todos los objetos
	/*!Libera la memoria ocupada por el arreglo de apuntadores y por cada uno de los
	objetos apuntados por estos.*/
    void FlushDestroy()
	{
		Truncar(0, true, true);
	}

private:

	//Cantidad de items que puede albergar el arreglo (memoria asignada a m_pData)
	int m_capacidad;

	//Cantidad de items existentes en el arreglo.
    int m_items;

	//Arreglo que contiene los apuntadores
    T* *m_pData;
};



//!Constructor por copia de otro Arreglo.
/*!Copia id�nticamente el arreglo origen, creando copias de los objetos 
a los que hacen referencia los apuntadores contenidos en �l. 
No se debe usar para arreglos de clases abstractas ya que no se pueden  
crear instancias de estas clases.
\param origen Objeto del que se hace copia.*/
template<class T>
Arreglo<T>::Arreglo(const Arreglo<T>& origen)
{
	m_capacidad=0;
	m_items=0;
	m_pData=NULL;
	operator=(origen);
}


//!Operador de asignaci�n a partir de otro arreglo
/*!Copia id�nticamente el arreglo origen, creando copias de los objetos 
a los que hacen referencia los apuntadores contenidos en �l. 
No se debe usar para arreglos de clases abstractas ya que no se pueden  
crear instancias de estas clases.
\param origen Objeto del que se hace copia.
\return Referencia al arreglo*/
template<class T>
const Arreglo<T>& Arreglo<T>:: operator =(const Arreglo<T>& origen)
{
	int tam = origen.getSize();
	asignarMemoria(tam);
	int i, miTam=m_items;
	for(i=0; i<tam && i<miTam; i++)
	{
		*(m_pData[i]) = *(origen.m_pData[i]);
	}
	for(i=miTam; i<tam; i++)
	{		
		T* pNuevo = new T(*(origen.m_pData[i]) );
		m_pData[i]=pNuevo;
	}
	if(miTam>tam)
		Truncar(tam, true, true);
	else
		m_items=tam;
	return *this;
}


/*!Todos los miembros a partir de la posici�n de inserci�n aumentan una posici�n.
En caso de estar lleno el arreglo, realiza asignaci�n de espacio para 20 items m�s.
\param pNuevo Apuntador al nuevo miembro del arreglo.
\param pos Posici�n en la que se ubicar� el nuevo miembro. Si es menor que cero, es ubicado en la posici�n cero. Si es mayor que el n�mero de items, se adiciona al final del arreglo.
\return Posici�n en la que fu� ubicado el nuevo miembro.
*/
template <class T>
int Arreglo<T>::Insertar(T* pNuevo, int pos)
{
    if(pos<0){pos=0;}
    if(pos>=m_items) return Adicionar(pNuevo);
	if(m_items >= m_capacidad)
		asignarMemoria(m_items+20);
	memmove( m_pData+pos+1  , m_pData+pos, (m_items-pos)*sizeof(T*) );
	m_pData[pos] = pNuevo;
	m_items++;
	return pos;
}



/*!Utilice liberar_memoria=false si sabe que va a volver a usar la capacidad actual
del arreglo para evitar reasignaci�n posterior de memoria.
\param nuevoTam Nuevo tama�o del arreglo. Si es mayor que el actual, nada ocurre. Si es menor que cero, el nuevo tama�o es cero. 
\param eliminarObjetos Si es \a true se eliminan los objetos sobrantes. En caso contrario, los objetos dejan de ser parte del arreglo pero no son eliminados.
\param liberar_memoria Si es \a true se libera la memoria no necesaria.
\return Nuevo tama�o del arreglo. */
template<class T>
int Arreglo<T>::Truncar(int nuevoTam, bool eliminarObjetos, bool liberar_memoria)
{	
	if(nuevoTam>=m_items ) return m_items;	//no hay nada que cambiar. No se pueden adicionar objetos
	if( nuevoTam < 0)	nuevoTam = 0;	
	if(eliminarObjetos)
		for(int i=nuevoTam; i<m_items; i++) delete m_pData[i];
	m_items=nuevoTam;
	if(liberar_memoria) liberarMemoria();
	return m_items;
}


/*!Es conveniente invocar esta funci�n antes de adicionar nuevos miembros 
al arreglo cuando es conocido previamente el n�mero de �tems a adicionar 
ya que evita reasignaci�n de memoria en cada adici�n de miembros al arreglo.
\param n Nueva capacidad total del arreglo. Si el arreglo ya tiene suficiente memoria para albergar el \a n items, nada ocurre.
\return true si la memoria fue asignada correctamente. false en caso contrario*/
template <class T>
bool Arreglo<T>::asignarMemoria(int n)
{
	if(n <= m_capacidad) return true; //no hay necesidad de (re)asignar espacio
	
	T** pNuevo=new T*[n];
	if(pNuevo)
	{
		m_capacidad = n;
		if(m_items>0)
		{
			//copiar memoria a la nueva ubicaci�n
			memcpy(pNuevo, m_pData, m_items*sizeof(T*));
			delete [] m_pData;
		}
		m_pData = pNuevo;
		return true;
	}
	else
		return false;
}


/*!Se utiliza para liberar la memoria no utilizada por el arreglo*/
template <class T>
void Arreglo<T>::liberarMemoria()
{
	if( m_items >= m_capacidad ) return; //no hay memoria que liberar
	int nuevaCap = m_items;
	if(nuevaCap==0) nuevaCap=1;
	T** pNuevo=new T*[nuevaCap];
	if(pNuevo)
	{
		m_capacidad = nuevaCap;
		//copiar memoria a la nueva ubicaci�n
		memcpy(pNuevo, m_pData, nuevaCap*sizeof(T*));
		delete [] m_pData;
		m_pData = pNuevo;
	}
}


/*!Todos los miembros del arreglo que se encuentran en posiciones mayores que 
\a pos, disminuyen una posici�n.
\param pos Posici�n del apuntador a excluir del arreglo.
\return Apuntador al objeto excluido del arreglo. Si \a pos no es una posici�n v�lida retorna NULL*/
template <class T>
T* Arreglo<T>::Detach (int pos)
{
	if(pos<0 || pos>=m_items) return NULL;
	T* ptr=m_pData[pos];
	m_items--;
	//mover memoria una posici�n
	if(pos < m_items )
		memmove( m_pData+pos  , m_pData+pos+1, (m_items-pos)*sizeof(T*));
	return ptr;
}


/*!Todos los miembros del arreglo que se encuentran en posiciones mayores que 
\a pos, disminuyen una posici�n.
\param pos Posici�n del apuntador a excluir del arreglo.*/
template <class T>
void Arreglo<T>::Destroy (int pos)
{
	if (pos < 0 || pos >= m_items) return;
    T* ptr=m_pData[pos];
	m_items--;
    //mover memoria una posici�n
	if(pos < m_items )
		memmove( m_pData+pos  , m_pData+pos+1, (m_items-pos)*sizeof(T*));
    delete ptr;
}



#endif		/*__ARREGLOS_H*/