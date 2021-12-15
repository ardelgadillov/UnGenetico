#ifndef __GENENTERO_H
#define __GENENTERO_H


#include "genetico.h"


/*****************************************************
               GEN ENTERO (long)
*****************************************************/

//!Clase derivada de la clase Gen especializada en un gen de tipo entero
/*!Se utiliza para representar y almacenar la informaci�n gen�tica de variables
cuya codificaci�n se realiza con n�meros enteros.*/
class GenEntero:public Gen
{
public:
	
	//!Constructor por copia
	/*!Construye el nuevo objeto copiando id�nticamente las propiedades 
	de otro objeto de la clase GenEntero.
	\param origen Objeto del que se hace copia.*/
	GenEntero(const GenEntero& origen)
	{
		operator=(origen);
	}

	//!Constructor por defecto
	GenEntero(long Min=-10, long Max=10);

	//!Constructor con valor inicial
	GenEntero(long Min, long Max, long ValorInicial);

	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenEntero id�ntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenEntero(*this));
	}

	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const GenEntero*)(&origen)));
	}

	//!Operador de asignaci�n a partir de un dato entero.
	/*!Asigna al gen el valor de un dato de tipo entero. 
	\param nuevoValor N�mero entero que se asigna al gen
	\return Referencia al gen.*/
    const GenEntero& operator=(const long& nuevoValor)
	{
		setVal(nuevoValor);
		return *this;
	}

	//!Conversi�n impl�cita a long
	/*!Convierte un objeto GenEntero en un dato de tipo long 
	utilizando su valor actual. Permite hacer operaciones del estilo \f$(long)gen1+(long)gen2\f$.
	\return Valor actual del gen.*/
    operator const long() const 
	{
		return m_Valor; 
	}

	//!Destructor
	~GenEntero(){}

	//!Ajusta el valor actual del dato almacenado por el gen. Retorna el valor asignado.
	/*!
	\param nuevoValor Valor a asignar al gen. Es restringido al rango <i>[m_Minimo, m_Maximo]</i>.
	\return Valor asignado.*/
	long setVal(long nuevoValor)
	{
		return m_Valor = restringir(nuevoValor, m_Minimo, m_Maximo);
	}

	//!Retorna el valor actual del dato almacenado en el gen
	/*!
	\return Valor actual del gen */
	long getVal() const
	{
		return m_Valor;
	}

	//!Retorna el valor m�ximo que puede tomar el gen
	/*!
	\return Valor m�ximo permitido para el gen. */
	long getMax() const
	{
		return m_Maximo;
	}

	//!Retorna el valor m�nimo que puede tomar el gen
	/*!
	\return Valor m�nimo permitido para el gen. */
	long getMin() const
	{
		return m_Minimo;
	}

	//!Genera un valor aleatorio para el dato almacenado en el gen
	/*!Asigna al gen un nuevo valor entero aleatorio en el rango establecido*/
	void generarAleatorio()
	{
		double azar = (double)rand()/(double)RAND_MAX;
		m_Valor = redondear( m_Minimo + azar*(m_Maximo-m_Minimo));	
	}

	//!Retorna un objeto correspondiente al operador de mutaci�n establecido por defecto para genes enteros
	inline OperadorMutacion *operadorMutacionDefecto() const;

	//!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes enteros
	inline OperadorCruce *operadorCruceDefecto() const;

protected:
	
	//!Almacena la informaci�n del gen
	long m_Valor;
    
	//!M�nimo valor que puede tomar el gen. Solo puede modificarse en el constructor
	long m_Minimo;
	
	//!M�ximo valor que puede tomar el gen. Solo puede modificarse en el constructor	
	long m_Maximo;	
};


/*****************************************************
                Operadores de Mutaci�n
*****************************************************/


//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efect�a una mutaci�n uniforme sobre un gen de tipo entero*/
class OperadorMutacionEnteroUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n.
	\param ProbabilidadMutacion Establece el valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1.*/
	OperadorMutacionEnteroUniforme(double ProbabilidadMutacion=0.1):OperadorMutacion(ProbabilidadMutacion){}

	//!Destructor
	~OperadorMutacionEnteroUniforme(){}

	//!Ejecuta una mutaci�n uniforme sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);
};


//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efect�a una mutaci�n no uniforme sobre un gen de tipo entero*/
class OperadorMutacionEnteroNoUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n y el valor de b*******************.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1
	\param b Valor Valor que determina el grado de dependencia con el numero de generaciones. Por defecto es 0.5 */
	OperadorMutacionEnteroNoUniforme(AlgoritmoGenetico *pAG, double ProbabilidadMutacion=0.1, double b=0.5):OperadorMutacion(ProbabilidadMutacion)
    {
		m_pAG=pAG;        
		setB(b);
    }

	//!Cambia el valor del par�metro \a b
	/*!\param b Valor a asignar al par�metro \a b.*/ 
	void setB(double b)
	{
		m_b = b;
	}

	//!Retorna el valor del par�metro \a b
	double getB() const
	{
		return m_b;
	}
	
	//!Destructor
	~OperadorMutacionEnteroNoUniforme(){}

	//!Ejecuta una mutaci�n no uniforme sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);

private:

	//!*Par�metro que determina el grado de dependencia con el numero de generaciones. 
	double m_b;	

	//!Apuntador a un objeto de la clase AlgoritmoGenetico
	AlgoritmoGenetico *m_pAG;
};

//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efect�a una mutaci�n Muhlenbein sobre un gen de de tipo entero*/
class OperadorMutacionEnteroMuhlenbein:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n y el valor del factor que define el rango de mutaci�n.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1.
	\param Factor Valor que define el rango de mutaci�n. Por defecto es 0.1.*/
	OperadorMutacionEnteroMuhlenbein(double ProbabilidadMutacion=0.1, double Factor=0.1):OperadorMutacion(ProbabilidadMutacion)
	{		
		setFactor(Factor);
	}	

	//!Cambia el valor del par�metro \a Factor
	/*!\param Factor Valor que define el rango de mutaci�n.*/
	void setFactor(double Factor)
	{
		m_Factor=Factor;
	}

	//!Retorna el valor del par�metro Factor
	double getFactor() const
	{
		return m_Factor;
	}

	//!Destructor
	~OperadorMutacionEnteroMuhlenbein(){}

	//!Ejecuta una mutaci�n Muhlenbein sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);

private:

	//!Valor que define el rango de mutaci�n. Solo puede modificarse en el constructor
	double m_Factor;
};


/*****************************************************
                Operadores de Cruce
*****************************************************/

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce plano entre dos genes de tipo entero*/
class OperadorCruceEnteroPlano:public OperadorCruce
{
public:
    //!Constructor
    OperadorCruceEnteroPlano(){}

    //!Destructor
    ~OperadorCruceEnteroPlano(){}

    //!Ejecuta un cruce plano sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce aritm�tico entre dos genes de tipo entero*/
class OperadorCruceEnteroAritmetico:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa el valor del par�metro \a Lambda.
	\param Lambda Valor que pondera cada uno de los genes padres. Por defecto es igual a 0.7.*/
	OperadorCruceEnteroAritmetico(double Lambda=0.7)
	{
		setLambda(Lambda);
	}
	
	//!Cambia el valor del par�metro \a Lambda comprobando los l�mites
	/*!\param Lambda Valor a asignar al par�metro \a Lambda.*/
	void setLambda(double Lambda)
	{
		m_Lambda = restringir(Lambda, 0.0, 1.0);
	}
	
	//!Retorna el valor del par�metro \a Lambda
	double getLambda()
	{
		return m_Lambda;
	}
	
	//!Destructor
	~OperadorCruceEnteroAritmetico(){}	
	
	//!Ejecuta un cruce aritm�tico sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);

private:
	//!Par�metro que pondera cada uno de los genes padres.
	double m_Lambda;
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce BLX -\f$\alpha\f$ entre dos genes de tipo entero*/
class OperadorCruceEnteroBLX:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa Inicializa el valor de \a Alfa
	\param Alfa Establece la amplitud del intervalo de definici�n. Por defecto es igual a 0.3.*/
	OperadorCruceEnteroBLX(double Alfa=0.3)
	{
		setAlfa(Alfa);
	}
	//!Cambia el valor del par�metro \a Alfa comprobando los l�mites
	/*!\param Alfa Valor a asignar al par�metro \a Alfa*/
	void setAlfa(double Alfa)
	{
		m_Alfa = restringir(Alfa, 0.0, 1.0);
	}
	//!Retorna el valor del par�metro \a Alfa
	double getAlfa()
	{
		return m_Alfa;
	}
	//!Destructor
	~OperadorCruceEnteroBLX(){}
	//!Ejecuta un cruce BLX - \f$\alpha\f$ sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Establece la amplitud del intervalo de definici�n.
	double m_Alfa;
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce lineal entre dos genes de tipo entero*/
class OperadorCruceEnteroLineal:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroLineal(){}
	//!Destructor 
	~OperadorCruceEnteroLineal(){}
	//!Ejecuta un cruce lineal sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce discreto entre dos genes de tipo entero*/
class OperadorCruceEnteroDiscreto:public OperadorCruce
{
public:
  //!Constructor
    OperadorCruceEnteroDiscreto(){}
  //!Destructor
    ~OperadorCruceEnteroDiscreto(){}
  //!Ejecuta un cruce discreto sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce extendido intermedio entre dos genes de tipo entero*/
class OperadorCruceEnteroIntermedioExtendido:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroIntermedioExtendido(){}
	//!Destructor
	~OperadorCruceEnteroIntermedioExtendido(){}
	//!Ejecuta un cruce intermedio extendido sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce heur�stico entre dos genes de tipo entero*/
class OperadorCruceEnteroHeuristico:public OperadorCruce
{
public:
  //!Constructor
    OperadorCruceEnteroHeuristico(){}
  //!Destructor
    ~OperadorCruceEnteroHeuristico(){}
  //!Ejecuta un cruce heur�stico sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};
 
//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efect�a un cruce BGA lineal entre dos genes de tipo entero*/
class OperadorCruceEnteroLinealBGA:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroLinealBGA(AlgoritmoGenetico *pAG){m_pAG=pAG;}
	//!Destructor
	~OperadorCruceEnteroLinealBGA(){}
	//!Ejecuta el cruce lineal BGA sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Apuntador al objeto AlgoritmoGenetico en el que opera
	AlgoritmoGenetico *m_pAG;
};



/****************************************************************************/
/*		DEFINICION DE MACROS PARA USO DE GENENTERO					
/****************************************************************************/

//!Macro que adiciona un gen de tipo entero a un individuo
/*!
Se debe utilizar solamente en la funci�n codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su prop�sito particular es insertar un objeto GenEntero en una posici�n determinada
del individuo apuntado por \a pIndividuo.
Este objeto estar� asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico.
De esta variable se establecen los valores m�nimo, m�ximo e inicial.

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posici�n en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado.
\param valMin Valor m�nimo que puede tomar el gen.
\param valMax Valor m�ximo que puede tomar el gen.
\param valInicial Valor inicial que toma el gen.*/
#define ADICIONAR_GENENTERO(pIndividuo, pos, var, valMin, valMax, valInicial)	\
	__ADICIONAR_GEN(pIndividuo, pos, var, long, GenEntero, (valMin, valMax, valInicial) )


#endif		//__GENENTERO_H