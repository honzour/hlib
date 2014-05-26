#ifndef HoReaderH
#define HoReaderH
/**
  Abstraktni trida (rozhrani) reprezentujici reader, napr. pulku pipy.
*/
class HoReader {
public:
	/**
	Prazdny konstruktor.
	*/
	HoReader(){};
	/**
	Cte data.
	@param size pocet pozadovanych bytu
	@param data buffer pro prectena data
	@return pocet skutecne prectenych bytu nebo -1 pokud chyba
	*/
	virtual int read(int size, void *data) = 0;
	/**
	Prazdny destruktor.
	*/
	virtual ~HoReader(){};
};
#endif
