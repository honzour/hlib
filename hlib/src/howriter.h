#ifndef HoWriterH
#define HoWriterH
/**
  Abstraktni trida (rozhrani) reprezentujici writer, napr. pulku pipy.
*/
class HoWriter {
public:
	/**
	Prazdny destruktor.
	*/
	HoWriter(){};
	/**
	Zpise data.
	@param size pocet pozadovanych bytu
	@param data buffer pro zapsana data
	@return pocet skutecne zapsanych bytu nebo -1 pokud chyba
	*/
	virtual int write(int size, void *data) = 0;
	/**
	Prazdny destruktor.
	*/
	virtual ~HoWriter(){};
};
#endif
