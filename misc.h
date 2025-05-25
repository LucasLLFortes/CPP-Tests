#ifndef MISC_H
#define MISC_H

#include <algorithm>
#include <functional>
#include <string>
#include <list>
#include <vector>

#define BUFFER_SIZE    1024 // 512

struct eqint {
    bool operator()(int s1, int s2) const {
        return s1 == s2;
    }
};

//added to compile linetestmanager-------
bool isConversibleToFloat(const std::string &strNumber);
std::string getTokenByIndex(const std::string &line, const int index);
std::string DoubleToStr(double d);
size_t moveToNextNonBlank(const std::string &text, const size_t startPos);
void stripCtrlM(std::string &str);
void removeEspacos(std::string &str);
const std::vector<std::string> SplitVector(const std::string &data, const std::string &separator);
size_t moveToNextBlank(const std::string &text, const size_t startPos);

short StrToShort(std::string s);
int StrToInt(std::string s);
double StrToDouble(std::string s);
long int StrToLong(std::string s);
unsigned int StrToUnsignedInt(std::string s);
unsigned long int StrToUnsignedLong(std::string s);
double StrToDouble(std::string s);
std::string ShortToStr(short n);
std::string IntToStr(int n);
std::string LongToStr(long int n);
std::string UnsignedLongToStr(unsigned long int n);
void StrToUpper(std::string& str);
std::string ReturnUpperValue(std::string str);
std::list<std::string> *Split(std::string data, char separator);
std::list<std::string> *Split(std::string data, std::string separator = " ");
std::vector<std::string> *VSplit(std::string data, std::string separator  = " ");
std::string Join(std::list<std::string> *data, std::string separator);
void EscapeString(char *dest, const char *src, bool escape = false);
void escapeString(char *buffer, int *bufferSize);
std::string escapeString(const std::string &str,const std::string &escapeChars = "#",bool escape = true);
void printBuffer(const char *buffer, int bufferSize);
int countCharacter(char cfind, char *buffer, int bufferSize);
// Returns -1 if text is not found.
// Otherwise, returns the initial position where the text was found.
int matchString(const char *buffer,const char *textToMatch, int bufferSize);
void makeBuffer7E1(char *buffer, int buffersize);
void makeBuffer7N0(char *buffer, int buffersize);
void RTrim(std::string &text);
void LTrim(std::string &text);
void Trim(std::string &text,char delimiter='\0');
void ToUpper(std::string &str);
void ToLower(std::string &str);
char *ToLower(const char *str);
std::string MakePassword( int size );
std::string ExtractFileName(const std::string &filePath);
char* itoa( int value, char* result, int base ); 
std::string itoa(int value, int base);
struct tm stringToDate(const std::string date);

namespace ATI {
  template<typename ForwardIt, typename OutputIt, typename UnaryPred>
    OutputIt copy_if(ForwardIt begin, ForwardIt end, OutputIt dest, UnaryPred f)
    {
    while(begin != end)
    {

        if(f(*begin))
        *dest++ = *begin;
        begin++;
    }
    return dest;
    }
}
// Remove '\r' e '\n' do fim da std::string
void chop(std::string& line);
int replace_str(std::string &str, const std::string before, const std::string after);
const std::string StrError(int err);
std::string DateFormat(const struct tm &a_tmDate, const std::string &a_sFormat);
//#define MIN(a,b) (a < b) ? a : b
bool strEqual(const std::string &str1, const std::string &str2);
unsigned long SecondsAfter2016();

bool isNaN(float &v);
bool isNaN(double &v);

bool is_ipv4_address(char const* ip);
bool is_number(char const* c, int &pos, int &value);
bool is_hexnumber(char const* c, size_t &pos, size_t &value);

// Pesquisa no vetor dentro de [begin, end) a primeia ocorrencia de "value"
// Troca essa ocorrencia com a última posição do vetor (que é vec.end()-1)
// Ṕor fim, remove essa ocorrencia
//
// Obs: o método pode desordenar um vetor ordenado
template<class T, class Allocator, class UnaryOp>
std::enable_if_t<std::is_invocable_v<UnaryOp, T>, void>
swap_and_remove(std::vector<T, Allocator> &vec, UnaryOp&& value)
{
    if(!vec.size()) return;

    // Posicao inicial a ser verificada
    size_t pos_end = vec.size()-1;

    for(size_t i = 0; i < pos_end; i++)
    {
        // Valor encontrado
        if(std::forward<UnaryOp>(value)(vec[i]))
        {
            vec[i] = std::move(vec[pos_end]);
            vec.pop_back();

            return;
        }
    }

    if(std::forward<UnaryOp>(value)(vec[pos_end])) vec.pop_back();
}

// Pesquisa em todo o vetor a primeia ocorrencia de "value"
// Troca essa ocorrencia com a última posição do vetor (que é vec.end()-1)
// Ṕor fim, remove essa ocorrencia
//
// Obs: o método pode desordenar um vetor ordenado
template<class T, class Allocator,
    bool isFund = std::is_fundamental_v<T>,
    class Type = typename std::conditional_t<isFund, T, T&>
>
std::enable_if_t<
    (isFund && std::is_same_v<Type, T>) ||
    (!isFund && std::is_same_v<std::remove_reference_t<Type>, T>),
void>
swap_and_remove(std::vector<T, Allocator> &vec, const Type value)
{   
    for(size_t i = 0; i < vec.size(); i++)
    {
        if(vec[i] == value)
        {
            vec[i] = std::move(vec[vec.size()]);
            vec.pop_back();

            return;
        }
    }
}

// Troca o iterador com o elemento na ultima posicao e então
// o remove
//
// Obs: o método pode desordenar um vetor ordenado
template<class T, class Allocator, class InputIt>
std::enable_if_t<
    std::is_same_v<InputIt, typename std::vector<T, Allocator>::iterator>,void>
swap_and_remove(std::vector<T, Allocator> &vec, const InputIt it)
{
    // Posicoes do vetor
    size_t pos_begin = std::distance(vec.begin(), it);
    size_t pos_end = vec.size()-1;

    if(pos_begin != pos_end) vec[pos_begin] = std::move(vec[pos_end]);
    vec.pop_back();
}

namespace detail {

    // NÃO UTILIZAR!
    // Pesquisa em todo no vetor de todas ocorrencias de "value"
    // Troca essas ocorrencias com as últimas posições do vetor
    // Ṕor fim, remove essas ocorrencias
    //
    // Obs: o método pode desordenar um vetor ordenado
    // Type pode ser igual a T, T&, ou um UnaryPred (metodo de comparação)
    template<class Type, class T, class Allocator, class InputIt>
    void swap_and_remove_all_intern(std::vector<T, Allocator> &vec, 
                                        const Type value, InputIt begin)
    {
        size_t size = vec.size();
        if(size == 0) return;

        // Primeira posição que sera removida
        size_t posRemove = size;

        // Ultima posicao valida do vetor
        size_t pos_end = size-1;

        // Posicao atual
        size_t pos;

        // Um iterador do vetor, que começa em begin
        for(pos = 0; pos != pos_end; pos++)
        {
            bool result;
            if constexpr (!std::is_invocable_v<Type, T>) result = (vec[pos] == value);
            else result = std::forward<Type>(value)(vec[pos_end]);

            // Verifica se o valor da posicao é igual a comparação, e se for,
            // pega o proximo elemento do final vetor para essa posicao
            if(result)
            {
                posRemove--;
                vec[pos] = std::move(vec[posRemove]);
            }
        }

        // Verifica a ultima posicao
        // Ela nao precisa sofrer nenhuma copia
        bool result;
        if constexpr (!std::is_invocable_v<Type, T>) result = (vec[pos_end] == value);
        else result = std::forward<Type>(value)(vec[pos_end]);

        if(result) posRemove--;

        // Retorna o iterador a partir de onde deve começar a apagar
        InputIt eraseStart = std::next(vec.begin(), posRemove);

        // Se os dois são iguais, o algoritmo apaga os dados e finaliza
        vec.erase(eraseStart, vec.end());
    }
}

// Pesquisa em todo no vetor de todas ocorrencias de "value"
// Troca essas ocorrencias com as últimas posições do vetor
// Ṕor fim, remove essas ocorrencias
//
// Obs: o método pode desordenar um vetor ordenado
template<class T, class Allocator, class InputIt,
    // Decide se e melhor utilizar passagem por referencia ou valor
    typename Type = typename std::conditional<std::is_fundamental<T>::value, T, T&>::type>
    // ...
typename std::enable_if_t<!std::is_invocable_v<Type, T>, void>
inline swap_and_remove_all(std::vector<T, Allocator> &vec, const Type value, InputIt begin)
{
    detail::swap_and_remove_all_intern<Type>(vec, value, begin);
}

template<class T, class Allocator,
typename Type = typename std::conditional<std::is_fundamental<T>::value, T, T&>::type>
typename std::enable_if_t<!std::is_invocable_v<Type, T>, void>
inline swap_and_remove_all(std::vector<T, Allocator> &vec, const Type value)
{
    swap_and_remove_all(vec, value, vec.begin());
}

// Pesquisa em todo o vetor pred(data) = true
// Troca essas ocorrencias com as últimas posições do vetor
// Ṕor fim, remove essas ocorrencias
//
// Obs: o método pode desordenar um vetor ordenado
template<class T, class Allocator, class InputIt, class UnaryPred>
typename std::enable_if_t<std::is_invocable_v<UnaryPred, T>, void>
inline swap_and_remove_all(std::vector<T, Allocator> &vec, UnaryPred&& pred, InputIt begin)
{
    detail::swap_and_remove_all_intern<UnaryPred&&>(vec, std::move(pred), begin);
}

template<class T, class Allocator, class UnaryPred>
typename std::enable_if_t<std::is_invocable_v<UnaryPred, T>, void>
inline swap_and_remove_all(std::vector<T, Allocator> &vec, UnaryPred&& pred)
{
    swap_and_remove_all(vec, std::move(pred), vec.begin());
}

#endif
