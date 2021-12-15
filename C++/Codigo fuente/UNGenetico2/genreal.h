#ifndef __GENREAL_H
#define __GENREAL_H

#include"genetico.h"

/************************************************************
                      GEN REAL (double)
************************************************************/

//!Clase derivada de la clase Gen especializada en un gen de tipo real
/*!Se utiliza para representar y almacenar la información genética de variables
cuya codificación se realiza con números reales.*/
class GenReal:public Gen
{
public:
	//!Constructor por copia
	/*!Construye el nuevo objeto copiando idénticamente las propiedades 
	de otro objeto de la clase GenReal.
	\param origen Objeto del que se hace copia.*/
	GenReal(const GenReal& origen)
	{
		operator=(origen);
	}

    //!Constructor por defecto
    GenReal(double Min=0.0, double Max=1.0);

    //!Constructor con valor inicial
    GenReal(double Min, double Max, double ValorInicial);

	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenReal idéntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenReal(*this));
	}

	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const GenReal*)(&origen))); 
	}

    //!Operador de asignación a partir de un dato real.
	/*!Asigna al gen el valor de un dato de tipo real. 
	\param nuevoValor Número real que se asigna al gen
	\return Referencia al gen.*/
    const GenReal& operator=(const double& nuevoValor)
	{
		setVal(nuevoValor);
		return *this;
	}
	
	//!Conversión implícita a double
	/*!Convierte un objeto GenReal en un dato de tipo double 
	utilizando su valor actual. Permite hacer operaciones del estilo \f$(double)gen1+(double)gen2\f$.
	\return Valor actual del gen.*/
    operator const double() const 
	{
		return m_Valor; 
	}

	//!Destructor
    ~GenReal(){}

	//!Ajusta el valor actual del dato almacenado por el gen. Retorna el valor asignado.
	/*!
	\param nuevoValor Valor a asignar al gen. Es restringido al rango [m_Minimo, m_Maximo].
	\return Valor asignado.*/
	double setVal(double nuevoValor)
	{	
		return m_Valor = restringir(nuevoValor, m_Minimo, m_Maximo);
	}


	//!Retorna el valor actual del dato almacenado en el gen
	/*!
	\return Valor actual del gen */
	double getVal() const
	{
		return m_Valor;
	}
	
	//!Retorna el valor máximo que puede tomar el gen
	/*!
	\return Valor máximo permitido para el gen. */
	double getMax() const
	{
		return m_Maximo;
	}

	//!Retorna el valor mínimo que puede tomar el gen
	/*!
	\return Valor mínimo permitido para el gen. */
	double getMin() const
	{
		return m_Minimo;
	}

	
    //!Genera un valor aleatorio para el dato almacenado en el gen
	/*!Asigna al gen un nuevo valor real aleatorio en el rango establecido*/
    void generarAleatorio()
	{
		double azar = (double)rand()/(double)RAND_MAX;
		m_Valor = m_Minimo + azar * (m_Maximo - m_Minimo);	
	}


    //!Retorna un objeto correspondiente al operador de mutación establecido por defecto para genes enteros
    inline OperadorMutacion *operadorMutacionDefecto() const;

    //!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes enteros
    inline OperadorCruce *operadorCruceDefecto() const;

protected:
	
	//!Almacena la Información del gen
    double m_Valor;

    //!Mínimo valor que puede tomar el gen. Solo puede modificarse en el constructor
    double m_Minimo;

    //!Máximo valor que puede tomar el gen. Solo puede modificarse en el constructor
    double m_Maximo;
};


/******************************************************************
                     Operadores de Mutación
******************************************************************/

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efectúa una mutación uniforme sobre un gen de tipo real*/
class OperadorMutacionRealUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación.
	\param ProbabilidadMutacion Establece el valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1.*/
    OperadorMutacionRealUniforme(double ProbabilidadMutacion=0.1):OperadorMutacion(ProbabilidadMutacion){}
	
	//!Destructor
    ~OperadorMutacionRealUniforme(){}
    //!Ejecuta una mutación uniforme sobre un objeto de la clase GenReal
    void mutarGen(Gen *pGen);
};

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efectúa una mutación no uniforme sobre un gen de tipo real*/
class OperadorMutacionRealNoUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación y el valor de b.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1
	\param b Valor que determina el grado de dependencia con el numero de generaciones. Por defecto es 0.5 */
    OperadorMutacionRealNoUniforme(AlgoritmoGenetico *pAG, double ProbabilidadMutacion=0.1, double b=0.5):OperadorMutacion(ProbabilidadMutacion)
    {
		m_pAG = pAG;
		setB(b);
    }
	
	//!Cambia el valor del parámetro \a b
	/*!\param b Valor a asignar al parámetro \a b.*/ 
	void setB(double b)
	{
		m_b = b;
	}
	//!Retorna el valor del parámetro \a b
	double getB()
	{
		return m_b;
	}
	//!Destructor
	~OperadorMutacionRealNoUniforme(){}
	
	//!Ejecuta una mutación no uniforme sobre un objeto de la clase GenReal
	void mutarGen(Gen *g);

private:
	
	//!Parámetro que determina el grado de dependencia con el numero de generaciones. 
	double m_b;
	
	//!Apuntador a un objeto de la clase AlgoritmoGenetico
	AlgoritmoGenetico *m_pAG;
};

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efectúa una mutación Muhlenbein sobre un gen de de tipo real*/
class OperadorMutacionRealMuhlenbein:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación y el valor del factor que define el rango de mutación.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1.
	\param Factor Valor que define el rango de mutación. Por defecto es 0.1.*/
	OperadorMutacionRealMuhlenbein(double ProbabilidadMutacion=0.1, double Factor=0.1):OperadorMutacion(ProbabilidadMutacion)
	{		
		setFactor(Factor);
	}	
	//!Cambia el valor del parámetro \a Factor
	/*!\param Factor Valor que define el rango de mutación.*/
	void setFactor(double Factor)
	{
		m_Factor=Factor;
	}
	//!Retorna el valor del parámetro Factor
	double getFactor()
	{
		return m_Factor;
	}
	//!Destructor 
	~OperadorMutacionRealMuhlenbein(){}  
	
	//!Ejecuta una mutación Muhlenbein sobre un objeto de la clase GenReal
	void mutarGen(Gen *pGen);

private:
	
	//!Valor que define el rango de mutación. 
	double m_Factor;
};

/*******************************************************************
                        Operadores de Cruce
*******************************************************************/

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce plano entre dos genes de tipo real*/
class OperadorCruceRealPlano : public OperadorCruce
{
public:
	//!Constructor
    OperadorCruceRealPlano(){}
	//!Destructor
    ~OperadorCruceRealPlano(){}
	//!Ejecuta un cruce plano sobre genes reales
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce aritmético entre dos genes de tipo real*/
class OperadorCruceRealAritmetico:public OperadorCruce
{
public:
	/*!Inicializa el valor del parámetro \a Lambda.
	\param Lambda Valor que pondera cada uno de los genes padres. Por defecto es igual a 0.7.*/
	OperadorCruceRealAritmetico(double Lambda=0.7)
	{
		setLambda(Lambda);
	}
	//!Cambia el valor del parámetro \a Lambda comprobando los límites
	/*!\param Lambda Valor a asignar al parámetro \a Lambda.*/
	void setLambda(double Lambda)
	{
		m_Lambda = restringir(Lambda, 0.0, 1.0);
	}
	//!Retorna el valor del parámetro \a Lambda
	double getLambda()
	{
		return m_Lambda;
	}
	//!Destructor 
	~OperadorCruceRealAritmetico(){}
	//!Ejecuta un cruce aritmético sobre genes reales.
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Parámetro que pondera cada uno de los genes padres.
	double m_Lambda;
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce BLX - \f$\alpha\f$ entre dos genes de tipo real*/
class OperadorCruceRealBLX:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa el valor de \a Alfa
	\param Alfa Establece la amplitud del intervalo de definición. Por defecto es igual a 0.3.*/
	OperadorCruceRealBLX(double Alfa=0.3)
	{
		setAlfa(Alfa);
	}
	//!Cambia el valor del parámetro \a Alfa comprobando los límites
	/*!\param Alfa Valor a asignar al parámetro \a Alfa*/
	void setAlfa(double Alfa)
	{
		m_Alfa = restringir(Alfa, 0.0, 1.0);
	}
	//!Retorna el valor del parámetro \a Alfa
	double getAlfa()
	{
		return m_Alfa;
	}
	//!Destructor 
	~OperadorCruceRealBLX(){}  
	//!Ejecuta el cruce BLX - \f$\alpha\f$ sobre genes reales.
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Establece la amplitud del intervalo de definición.
	double m_Alfa;
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce lineal entre dos genes de tipo real*/
class OperadorCruceRealLineal:public OperadorCruce
{
public:
	//!Constructor 
	OperadorCruceRealLineal(){}
	//!Destructor 
	~OperadorCruceRealLineal(){}
	//!Ejecuta un cruce lineal sobre genes reales
	void cruzarGenes(const Gen *madre, const Gen *padre, Arreglo<Gen> *hijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce discreto entre dos genes de tipo real*/
class OperadorCruceRealDiscreto:public OperadorCruce
{
public:
  //!Constructor 
  OperadorCruceRealDiscreto(){}
  //!Destructor 
  ~OperadorCruceRealDiscreto(){}
  //!Ejecuta un cruce discreto sobre genes reales
  void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa el cruce extendido intermedio entre dos genes de tipo real*/
class OperadorCruceRealIntermedioExtendido:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealIntermedioExtendido(){}
	//!Destructor
	~OperadorCruceRealIntermedioExtendido(){}
	//!Ejecuta un cruce intermedio extendido sobre genes reales
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce heurístico entre dos genes de tipo real*/
class OperadorCruceRealHeuristico:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealHeuristico(){}
	//!Destructor
	~OperadorCruceRealHeuristico(){}
	//!Ejecuta un cruce heurístico sobre genes reales
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efectúa un cruce BGA linealentre genes de tipo real*/
class OperadorCruceRealLinealBGA:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealLinealBGA(AlgoritmoGenetico *pAG){m_pAG=pAG;}
	//!Destructor
	~OperadorCruceRealLinealBGA(){}
	//!Ejecuta el cruce BGA lineal sobre genes reales
	void cruzarGenes(const Gen *madre, const Gen *padre, Arreglo<Gen> *hijos, int numHijos, int indice);
private:
	//!Apuntador al objeto AlgoritmoGenetico en el que opera
	AlgoritmoGenetico *m_pAG;
};



/****************************************************************************/
/*		DEFINICION DE MACROS PARA USO DE GENREAL				
/****************************************************************************/

//!Macro que adiciona un gen de tipo real al individuo
/*Usa la macro __ADICIONAR_GEN con el propósito particular de insertar
un objeto GenReal en una posición fija del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar
de la cual se establecen sus valores mínimo, máximo e inicial.

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado.
\param valMin Valor mínimo que puede tomar el gen.
\param valMax Valor máximo que puede tomar el gen.
\param valInicial Valor inicial que toma el gen.*/
#define ADICIONAR_GENREAL(pIndividuo, pos, var, valMin, valMax, valInicial)	\
	__ADICIONAR_GEN(pIndividuo, pos, var, double, GenReal,(valMin, valMax, valInicial))


#endif	//__GENREAL_H