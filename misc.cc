#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#include "Logger.h"

using namespace std;

//added to compile linetestmanager---------------------
bool isConversibleToFloat(const string &strNumber) {
    istringstream strStream(strNumber);
    float fltNumber;

    if (strStream >> fltNumber)
        return true;
    else
        return false;
}

string getTokenByIndex(const string &line, const int index) {
// This function receive a line and a index and return the token at this "index" position in this line.
// The tokens are separated by blank spaces.
    size_t x = 0, y = 0;
    int count = -1;
    string token;

    while ((x != string::npos) && (y != string::npos) && count != index) {
        x = moveToNextNonBlank(line, y);
        y = moveToNextBlank(line, x);
        count++;
    }
    if (index > count) {
        Logger::lprintf(LOCAL_DEBUG, "getTokenByIndex() - ERROR: Index %d is out of range!\n", index);
        return "";
    }
    token = line.substr(x, y - x);
    return token;
}

string DoubleToStr(double d) {
    std::ostringstream os;

    os << d;
    return std::string(os.str());
}

void stripCtrlM(std::string &str) {
  str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}

void removeEspacos(string &str) {
    string ret;
    for (string::iterator it=str.begin(); it!=str.end(); ++it) {
        if (*it != ' ') {
            ret.append(1, *it);
        }
    }
    str.assign(ret);
}

const vector<string> SplitVector(const string &data, const string &separator) {
    string internalData(data);
    vector<string> tokens;
    size_t pos;

    if (internalData.empty()) {
        goto end;
    }

    if (separator.empty()) {
        tokens.push_back(internalData);
        goto end;
    }

    internalData += separator;
    pos = internalData.find(separator);
    while (pos != string::npos) {
        tokens.push_back(internalData.substr(0, pos));
        internalData.erase(0, pos + separator.size());
        pos = internalData.find(separator);
    }

end:

    return tokens;
}

size_t moveToNextNonBlank(const string &text, const size_t startPos) {
// This function returns the next position different from blank space from startPos
    size_t x = 0;

    x = text.find_first_not_of(' ', startPos);

    return x;
}

size_t moveToNextBlank(const string &text, const size_t startPos) {
// This function returns the next position equal to blank space from startPos
    size_t x = 0;

    x = text.find_first_of(' ', startPos);

    return x;
}
//---------------------------------------------------

short StrToShort(string s) 
{
  std::stringstream ss;
  short returnValue = 0;
  ss.str(s);
  ss >> returnValue;
  return returnValue;
}

int StrToInt(string s) {

    int returnValue;

    returnValue = atoi(s.c_str());

    return returnValue;
}

long int StrToLong(string s) {

    long int returnValue;

    returnValue = atol(s.c_str());

    return returnValue;
}

unsigned int StrToUnsignedInt(string s)
{
    unsigned long int lreturnValue;
    unsigned int returnValue;

    lreturnValue = strtoul(s.c_str(), 0, 10);
    returnValue = lreturnValue;

    if (returnValue != lreturnValue) 
        throw std::out_of_range("Out of range exception");

    return returnValue;
}

unsigned long int StrToUnsignedLong(string s) {

    unsigned long int returnValue;

    returnValue = strtoul(s.c_str(), 0, 10);

    return returnValue;
}

double StrToDouble(string s) {

    double returnValue;

    returnValue = stod(s.c_str());

    return returnValue;
}

string ShortToStr(short n) {
    std::ostringstream os;

    os << n;
    return std::string(os.str());
}

string IntToStr(int n)
{
    std::ostringstream os;
    os<<n;
    return std::string(os.str());
}

string LongToStr(long int n)
{
    std::ostringstream os;
    os<<n;
    return os.str();
}

string UnsignedLongToStr(unsigned long int n)
{
    std::ostringstream os;
    os<<n;
    return os.str();
}

void StrToUpper(string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

string ReturnUpperValue(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

list<string> *Split(string data, char separator)
{
 return Split(data,string(1,separator));
}

list<string> *Split(string data, string separator )
{
  //Logger::lprintf(DEBUG,"Split() - begin\n");
  
  list<string>      *tokens;
  size_t            pos;
  
  if (data.size() == 0)
    return NULL;
   
  tokens = new list<string>;
  
  data += separator; //sentinela
  pos = data.find(separator);
  while (pos != string::npos)
   {
     tokens->push_back(data.substr(0,pos));
     data.erase(0,pos+separator.size());
     pos = data.find(separator);
   }
   /*if( !data.empty() )
   tokens->push_back(data);*/
  
  //Logger::lprintf(DEBUG,"Split() - [%d] end \n",tokens->size());
  
  return tokens;
}

vector<string> *VSplit(string data, string separator )
{
  //Logger::lprintf(DEBUG,"Split() - begin\n");
  
  vector<string>      *tokens;
  size_t            pos;
  
  Trim(data);
  
  if (data.size() == 0)
    return NULL;
   
  tokens = new vector<string>;
  
  data += separator; //sentinela
  pos = data.find(separator);
  while (pos != string::npos)
   {
      //if(!data.substr(0,pos).empty()){ // com esta linha comentada sao aceitos elementos vazios
        tokens->push_back(data.substr(0,pos));
        data.erase(0,pos+separator.size());
        pos = data.find(separator);
      //}else{
       //break;   
      //}
   }
   /*if( !data.empty() )
   tokens->push_back(data);*/
  
  //Logger::lprintf(DEBUG,"Split() - [%d] end \n",tokens->size());
  
  return tokens;
}

string Join(list<string> *data, string separator) {
  string output;

  for(list<string>::iterator i=data->begin(); i!=data->end(); i++) {
    output += (*i) + separator;
  }
  if(output.size())
    output.erase(output.size()-separator.size());
  return output;

}

void EscapeString(char *dest, const char *src, bool escape)
{
    char aux[BUFFER_SIZE];
    int j;
    unsigned int i;

    j = 0;
    for (i = 0; i < strlen(src); i++) {
        if ((src[i] == '#') || (src[i] == ' ') || (escape && src[i] == '\\')) {
            aux[j] = '\\';
            aux[++j] = src[i];
        } else {
            aux[j] = src[i];
        }
        j++;

    }
    aux[j] = '\0';
    strcpy(dest,aux);
}

// It is necessary to garantee that buffer has a size that supports
// the incoming \ to escape the #. This method does NOT allocate
// more space to the \ to escape the #.
// So, if it receives ###bbb# it changes the buffer to \#\#\#bbb\#
void escapeString(char *buffer, int *bufferSize) {

    for (int i = 0; i < *bufferSize; i++) {
        if (buffer[i] == '#') {
            memmove(buffer+i+1, buffer+i, (*bufferSize)-i);
            (*bufferSize)++;
            buffer[i++] = '\\';
        }
    }
}

string escapeString(const string &str,const string &escapedChars, bool escape)
{
  size_t pos = 0;
  string aux = string(str.c_str(), str.size());
 
  if(str.empty())
    return aux;

  if(escape)
  { 
    while(pos < aux.size() && (pos = aux.find_first_of(escapedChars,pos)) != string::npos)
    {
      aux.insert(pos,"\\");
      pos = pos+2;
    }
  }
  else
  {
    while(pos < aux.size() && (pos = aux.find("\\",pos)) != string::npos)
    {
      if(escapedChars.find(aux[pos+1]))
      {
        aux.erase(pos,1);
        pos = pos+1;
      }
      else
      {
        Logger::lprintf(LOCAL_DEBUG,"Unknown escape sequence [\\%c]",aux[pos+1]);
        pos = pos+ 2;
      }
    }
  }

  return aux;
}

void printBuffer(const char *buffer, int bufferSize) {

    for (int i = 0; i < bufferSize; i++) {
        if (buffer[i] < ' ' || buffer[i] > '~') {
            printf("x%02X", (unsigned char)buffer[i]);
        } else {
            printf("%c", buffer[i]);
        }
    }
}

int countCharacter(char cfind, char *buffer, int bufferSize) {

    int count = 0;

    for (int i = 0; i < bufferSize; i++) {
        if (buffer[i] == cfind) {
            count++;
        }
    }
    return count;
}


int matchString(const char *buffer,const char *textToMatch, int bufferSize) {

    int i, j;
    int textToMatchSize;

    textToMatchSize = strlen(textToMatch);
    if (textToMatchSize == 0) {
        return -1;
    }

    for (i = 0; i < bufferSize; i++) {
        if (buffer[i] == textToMatch[0]) {
            for (j = 1; j < textToMatchSize && (i + j) < bufferSize && buffer[i + j] == textToMatch[j]; j++);
            if (j == textToMatchSize) {
                // textToMatch found.
                Logger::lprintf(LOCAL_DEBUG, "matchString() - Text found: [%s]\n", textToMatch);
                return i;
            }
        }
    }
    return -1;
}

void makeBuffer7E1(char *buffer, int buffersize)
// Funcao que converte todos os caracteres do buffer para 7 bits, fazendo
// do oitavo o bit de paridade par
{
    char buffChar;
    int i, j;
    int bitCount;

    for (i = 0; i < buffersize; i++)     // varre todos os caracteres do buffer
     {
       bitCount = 0;
       buffChar = buffer[i] & 0x7f;      // despreza o oitavo bit
       for (j = 0; j < 7; j++)           // varre os sete bits significativos
        {
          if (buffChar & (1 << j))         // do caracter. Se o bit for 1, entao
            bitCount++;                    // incrementa biCount.
        }
       if (bitCount % 2)                 // se numero de bits '1' for impar
         buffChar |= 0x80;               // entao seta o bit de paridade.
       buffer[i] = buffChar;             // atualiza o caracter no buffer.
     }
}

void makeBuffer7N0(char *buffer, int buffersize)
// Funcao que converte todos os caracteres do buffer para 7 bits, fazendo
// do oitavo o bit '0'
{
    int i;

    for (i = 0; i < buffersize; i++)     // varre todos os caracteres do buffer
     {
       buffer[i] &= 0x7f;                //zerando o oitavo bit
     }
}

void RTrim(string &text)
{
//  Logger::lprintf(DEBUG,"RTrim - begin\n");
  int pos;
  pos = text.size()-1;
  while(pos >= 0 && !isgraph(text[pos]))
  {
    pos--;
  }
  if(pos < 0)
    text = "";
  else
    text.erase(pos+1);
//  Logger::lprintf(DEBUG,"RTrim - end\n");
}

void LTrim(string &text)
{
//  Logger::lprintf(DEBUG,"LTrim - begin\n");
  int pos, size;
  pos = 0;
  size = text.size();
  while(pos < size && !isgraph(text[pos]))
  {
//    Logger::lprintf(LOCAL_DEBUG,"LTrim - %c\n",text[pos]);
    pos++;
  }
  if(pos == 0)
    asm("NOP");
  else if(pos >= size)
    text = "";
  else
    text.erase(0, pos);
//  Logger::lprintf(DEBUG,"LTrim - end\n");
}

void Trim(string &text, char delimiter)
{
//  Logger::lprintf(LOCAL_DEBUG,"Trim - begin\n");
  size_t init,end;

  if(delimiter != '\0')
  {
    init = text.find_first_not_of(string(1,delimiter));
    init = (init == string::npos)? 0 : init;

    end  = text.find_last_not_of(delimiter);
    text = text.substr(init,(end - init)+1);
  }
  else
  {
    LTrim(text);
    RTrim(text);
  }
//  Logger::lprintf(LOCAL_DEBUG,"Trim - end\n");
}

void ToUpper(string &str)
{
  for(size_t i=0;i<str.size();i++)
    str[i] = (char) toupper(str[i]);
}

void ToLower(string &str)
{
  for(size_t i=0;i<str.size();i++)
    str[i] = (char) tolower(str[i]);
}

char *ToLower(const char *str)
{
  char *aux;
  int len = strlen(str);

  aux = (char *)malloc(sizeof(char)*(len+1));
  aux[len] = '\0';
  for(int i=0;i< len;i++)
    aux[i] = (char) tolower(str[i]);
  
  return aux;
}

//Gera uma senha aleatória de tamanho size
//de caracteres alphanuméricos.
string MakePassword( int size )
{
  char ci = 0;
  string passwd;
  bool hasAlpha=false,hasDigit=false;

  //From rand() man page
  //"If you want to generate a random integer between 1 and 10, you should always do 
  // it by using high-order bits, as in
  //  j=1+(int) (10.0*rand()/(RAND_MAX+1.0));
  //and never by anything resembling
  //  j=1+(rand() % 10);
  //(which uses lower-order bits)." 
  for( int i =0; i < size; i++ )
  {
    ci = 0;
    while ( !isalnum(ci) )
      ci = 48+(int)(122.0*rand()/(RAND_MAX+48.0));
    
    if( isalpha(ci) )
      hasAlpha = true;
    else if( isdigit(ci) )
      hasDigit = true;

    if(i == size-1 && (!hasDigit || !hasAlpha))
    {
      i--;
      continue;
    }
    passwd += ci;
  }
  return passwd;
}
//Recupera o nome do arquivo se o caminho de diretórios
string ExtractFileName(const string &filePath)
{
  size_t pos;
  string name;
  
  if( (pos = filePath.find_last_of("/")) != string::npos)
  {
    name = filePath.substr(pos+1);
  }
  else
  {
    name = filePath;
  }
  return name;
}

/**
 * C++ version char* style "itoa":
 */
char* itoa( int value, char* result, int base ) 
{
  static const string values = "0123456789abcdef";
  int size = 0;
  // check that the base if valid
  if (base < 2 || base > 16) { *result = 0; return result; }

  char* out = result;
  int quotient = value;
  do 
  {
    *out = values[ std::abs( quotient % base ) ];
    ++out;
    size++;
    quotient /= base;
  } while ( quotient );

  // Only apply negative sign for base 10
  if ( value < 0 && base == 10) *out++ = '-';

  printBuffer(out,size);
  
  std::reverse( result, out );
  //printBuffer(out,size);
  
  *out = 0;
  printBuffer(result,size);
  return result;
}

/**
 * C++ version std::string style "itoa":
 */
std::string itoa(int value, int base) 
{
  enum { kMaxDigits = 35 };
  std::string buf;
  buf.reserve( kMaxDigits ); // Pre-allocate enough space.

  // check that the base if valid
  if (base < 2 || base > 16) return buf;

  int quotient = value;

  // Translating number to string with base:
  do {
    buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
    quotient /= base;
  } while ( quotient );

  // Append the negative sign for base 10
  if ( value < 0 && base == 10) buf += '-';

  std::reverse( buf.begin(), buf.end() );
  return buf;
}

void chop(string& line)
{
  size_t pos;

  pos = line.find("\n");
  if (pos != string::npos)
    line.erase(pos,1);
  pos = line.find("\r");
  if (pos != string::npos)
    line.erase(pos,1);
}

int replace_str(string &str, const string before, const string after)
{
  size_t pos;
  int occours = 0;
  while( (pos = str.find(before)) != string::npos)
  {
    str.replace(pos, before.size(), after);
    occours++;
  }
  return occours;
}

const string StrError(int err)
{
  char buf[120];
  memset(buf,'\0',120);
  #pragma GCC diagnostic ignored "-Wunused-result"
  strerror_r(err, buf, 120);
  #pragma GCC diagnostic pop
  return string(buf);
}

string DateFormat(const struct tm &a_tmDate, const string &a_sFormat){
  char date[80];
  size_t size;
  size = strftime(date, 80, a_sFormat.c_str(),&a_tmDate);
  return string(date, size);
}

bool strEqual(const std::string &str1, const std::string &str2) {
  return (str1.compare(str2) == 0);
}

//seconds since January 1, 2016 in the current timezone
unsigned long SecondsAfter2016() {
  time_t rawtime;
  struct tm *timeinfo;
  double seconds;

  time(&rawtime);
  // get current timeinfo and modify it to January 1, 2000
  timeinfo = localtime(&rawtime);
  timeinfo->tm_year = 116; //2016 - 1900
  timeinfo->tm_mon = 0;
  timeinfo->tm_mday = 1;
  timeinfo->tm_hour = 0;
  timeinfo->tm_min = 0;
  timeinfo->tm_sec = 0;

  seconds = difftime(rawtime,mktime(timeinfo));
  return (unsigned long)seconds;
}

struct tm stringToDate(const std::string date) {
  struct tm result; 
  memset(&result, 0, sizeof(result)); 
  strptime(date.c_str(), "%Y%m%d", &result); 
 
  Logger::lprintf(LOCAL_DEBUG, "stringToDate() - date[%s]\n", asctime (&result)); 

  return result; 
}


/**
 * @brief Verifica se nao e um numero (NaN - Not a Number)
 * 
 * @param v Numero verificado
 * @return true Nao e um numero
 * @return false Numero
 */
bool isNaN(float &v)
{
    // Poderiamos utilizar o metodo std::isnan para fazer o mesmo processo
    // O problema e que este metodo ao ser otimizado por compiladores, pode
    // simplesmente deixar de funcionar! (segundo o proprio manual do GCC)
    // Por isso definimos nosso proprio metodo que verifica a propriedade de um valor ser numero

    // A definicao de NAN segue o padrao IEEE-754. Verificar para entender o retorno

    return ( (*reinterpret_cast<uint32_t*>(&v) & 0x7f800000) == 0x7f800000)
    && (*reinterpret_cast<uint32_t*>(&v) & 0x7fffff);
}

/**
 * @brief Verifica se nao e um numero (NaN - Not a Number)
 * 
 * @param v Numero verificado
 * @return true Nao e um numero
 * @return false Numero
 */
bool isNaN(double &v)
{
    // Poderiamos utilizar o metodo std::isnan para fazer o mesmo processo
    // O problema e que este metodo ao ser otimizado por compiladores, pode
    // simplesmente deixar de funcionar! (segundo o proprio manual do GCC)
    // Por isso definimos nosso proprio metodo que verifica a propriedade de um valor ser numero

    // A definicao de NAN segue o padrao IEEE-754. Verificar para entender o retorno

    return ( (*reinterpret_cast<std::uint64_t*>(&v) & 0x7ff0000000000000) == 0x7ff0000000000000)
    && (*reinterpret_cast<std::uint64_t*>(&v) & 0xfffffffffffff);
}

/**
 * @brief Checa se um IP esta no formato IPV4
 * 
 * @param ip Texto a ser checado
 * @return true Formato IPV4 valido
 * @return false Formato IPV4 invalido
 */
bool is_ipv4_address(char const* ip)
{

  // O formato IPV4 e dado por X.X.X.X, onde X e um numero de 0 a 255
  int value;

  for(int i = 0, cpos = 0, pos; i < 4; i++, cpos+=pos+1) {
    // Verifica se as posicoes da string sao numeros (pulando os pontos)
    // E tambem se estao dentro da range valida
    if(!is_number(ip+cpos, pos, value) || value < 0 || value > 255) return false;
  }

  // IP valido
  return true;;

}

/**
 * @brief Verifica se e um numero e retorna seu valor
 * 
 * @param c String a ser lida
 * @param size Indica a primeira posicao nao valida
 * @param value Valor retornado, caso seja um numero
 * @return true Numero
 * @return false Nao e um numero
 */
bool is_number(char const* c, int &pos, int &value)
{

  // Auxiliar de leitura
  char caux = c[0];

  // Nao e um numero
  if(caux < '0' || caux > '9') return false;

  // Inicializa os valores
  value = caux - '0';
  pos = 1;

  caux = c[1];

  while(caux >= '0' && caux <= '9')
  {

    // Atualiza o digito, movendo os anteriores para esquerda
    // e somando o novo digito lido
    value *= 10;
    value += caux - '0'; 

    caux = c[++pos];

  }

  // E um numero
  return true;
    
}

/**
 * @brief Verifica se e um numero e retorna seu valor
 * 
 * @param c String a ser lida
 * @param size Indica a primeira posicao nao valida
 * @param value Valor retornado, caso seja um numero
 * @return true Numero
 * @return false Nao e um numero
 */
bool is_hexnumber(char const* c, size_t &pos, size_t &value)
{

  // Auxiliar de leitura
  char caux = c[0];

  // Nao e um numero
  if(!isxdigit(caux)) return false;

  // Inicializa os valores
  pos = 0;
  value = 0;

  do
  {

    // Atualiza o digito, movendo os anteriores para esquerda
    // e somando o novo digito lido
    value <<= 4;
    if(caux >= '0' && caux <= '9') value |= caux - '0';
    else if(caux <= 'F') value |= caux - 'A' + 10;
    else value |= caux - 'a' + 10;

    caux = c[++pos];

  }
  while(isxdigit(caux));

  // E um numero
  return true;
    
}