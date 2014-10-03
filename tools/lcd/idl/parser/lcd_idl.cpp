
 #include "lcd_ast.h"
	    #include <vector>
	    #include <stdlib.h>
	    #include <string>
	    #include <stdint.h> 

#include <list>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>


namespace Parser{



struct Value{
    typedef std::list<Value>::const_iterator iterator;

    Value():
        which(1),
        value(0){
    }

    Value(const Value & him):
    which(him.which),
    value(0){
        if (him.isData()){
            value = him.value;
        }
        if (him.isList()){
            values = him.values;
        }
    }

    explicit Value(const void * value):
        which(0),
        value(value){
    }

    Value & operator=(const Value & him){
        which = him.which;
        if (him.isData()){
            value = him.value;
        }
        if (him.isList()){
            values = him.values;
        }
        return *this;
    }

    Value & operator=(const void * what){
        this->value = what;
        return *this;
    }

    void reset(){
        this->value = 0;
        this->values.clear();
        this->which = 1;
    }

    int which; // 0 is value, 1 is values

    inline bool isList() const {
        return which == 1;
    }

    inline bool isData() const {
        return which == 0;
    }

    inline const void * getValue() const {
        return value;
    }

    inline void setValue(const void * value){
        which = 0;
        this->value = value;
    }

    inline const std::list<Value> & getValues() const {
        return values;
    }

    /*
    inline void setValues(std::list<Value> values){
        which = 1;
        values = values;
    }
    */

    const void * value;
    std::list<Value> values;
};

class Result{
public:
    Result():
    position(-2){
    }

    Result(const int position):
    position(position){
    }

    Result(const Result & r):
    position(r.position),
    value(r.value){
    }

    Result & operator=(const Result & r){
        position = r.position;
        value = r.value;
        return *this;
    }

    void reset(){
        value.reset();
    }

    void setPosition(int position){
        this->position = position;
    }

    inline int getPosition() const {
        return position;
    }

    inline bool error(){
        return position == -1;
    }

    inline bool calculated(){
        return position != -2;
    }

    inline void nextPosition(){
        position += 1;
    }

    void setError(){
        position = -1;
    }

    inline void setValue(const Value & value){
        this->value = value;
    }

    /*
    Value getLastValue() const {
        if (value.isList()){
            if (value.values.size() == 0){
                std::cout << "[peg] No last value to get!" << std::endl;
            }
            return value.values[value.values.size()-1];
        } else {
            return value;
        }
    }
    */

    inline int matches() const {
        if (value.isList()){
            return this->value.values.size();
        } else {
            return 1;
        }
    }

    inline const Value & getValues() const {
        return this->value;
    }

    void addResult(const Result & result){
        std::list<Value> & mine = this->value.values;
        mine.push_back(result.getValues());
        this->position = result.getPosition();
        this->value.which = 1;
    }

private:
    int position;
    Value value;
};



struct Chunk0{
Result chunk_Identifier;
    Result chunk_IdentCont;
    Result chunk_File;
    Result chunk_Typedef;
    Result chunk_Verbatim;
};

struct Chunk1{
Result chunk_RealType;
    Result chunk_String;
    Result chunk_Things;
    Result chunk_Include;
    Result chunk_FirstParam;
};

struct Chunk2{
Result chunk_LastParam;
    Result chunk_Parameters;
    Result chunk_Rpc;
    Result chunk_Message;
    Result chunk_Message_field;
};

struct Chunk3{
Result chunk_Projection;
    Result chunk_Proj_type;
    Result chunk_Start;
    Result chunk_Specification;
    Result chunk_ProjField;
};

struct Chunk4{
Result chunk_Last;
    Result chunk_Declaration;
    Result chunk_Comment;
    Result chunk_Comment_rest;
    Result chunk_Line_comm_rest;
};

struct Chunk5{
Result chunk_Line_comment;
    Result chunk_Simple_Type;
    Result chunk_Type_w_modifier;
    Result chunk_Type;
    Result chunk_Type_form;
};

struct Chunk6{
Result chunk_Spacing;
    Result chunk_Space;
};

struct Column{
Column():
    chunk0(0)
        ,chunk1(0)
        ,chunk2(0)
        ,chunk3(0)
        ,chunk4(0)
        ,chunk5(0)
        ,chunk6(0){
}

Chunk0 * chunk0;
    Chunk1 * chunk1;
    Chunk2 * chunk2;
    Chunk3 * chunk3;
    Chunk4 * chunk4;
    Chunk5 * chunk5;
    Chunk6 * chunk6;

int hitCount(){
    return 0;
}

int maxHits(){
    return 32;
}

~Column(){
    delete chunk0;
        delete chunk1;
        delete chunk2;
        delete chunk3;
        delete chunk4;
        delete chunk5;
        delete chunk6;
}
};


class ParseException: std::exception {
public:
    ParseException(const std::string & reason):
    std::exception(),
    line(-1), column(-1),
    message(reason){
    }

    ParseException(const std::string & reason, int line, int column):
    std::exception(),
    line(line), column(column),
    message(reason){
    }

    std::string getReason() const;
    int getLine() const;
    int getColumn() const;

    virtual ~ParseException() throw(){
    }

protected:
    int line, column;
    std::string message;
};

class Stream{
public:
    struct LineInfo{
        LineInfo(int line, int column):
        line(line),
        column(column){
        }

        LineInfo(const LineInfo & copy):
        line(copy.line),
        column(copy.column){
        }

        LineInfo():
        line(-1),
        column(-1){
        }

        int line;
        int column;
    };

public:
    /* read from a file */
    Stream(const std::string & filename):
    temp(0),
    buffer(0),
    farthest(0),
    last_line_info(-1){
        std::ifstream stream;
        /* ios::binary is needed on windows */
        stream.open(filename.c_str(), std::ios::in | std::ios::binary);
        if (stream.fail()){
            std::ostringstream out;
            out << __FILE__  << " cannot open '" << filename << "'";
            throw ParseException(out.str());
        }
        stream.seekg(0, std::ios_base::end);
        max = stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        temp = new char[max];
        stream.read(temp, max);
        buffer = temp;
        stream.close();

        line_info[-1] = LineInfo(1, 1);

        createMemo();
    }

    /* for null-terminated strings */
    Stream(const char * in):
    temp(0),
    buffer(in),
    farthest(0),
    last_line_info(-1){
        max = strlen(buffer);
        line_info[-1] = LineInfo(1, 1);
        createMemo();
    }

    /* user-defined length */
    Stream(const char * in, int length):
    temp(0),
    buffer(in),
    farthest(0),
    last_line_info(-1){
        max = length;
        line_info[-1] = LineInfo(1, 1);
        createMemo();
    }

    void createMemo(){
        memo_size = 1024 * 2;
        memo = new Column*[memo_size];
        /* dont create column objects before they are needed because transient
         * productions will never call for them so we can save some space by
         * not allocating columns at all.
         */
        memset(memo, 0, sizeof(Column*) * memo_size);
        /*
        for (int i = 0; i < memo_size; i++){
            memo[i] = new Column();
        }
        */
    }

    int length(){
        return max;
    }

    /* prints statistics about how often rules were fired and how
     * likely rules are to succeed
     */
    void printStats(){
        double min = 1;
        double max = 0;
        double average = 0;
        int count = 0;
        for (int i = 0; i < length(); i++){
            Column & c = getColumn(i);
            double rate = (double) c.hitCount() / (double) c.maxHits();
            if (rate != 0 && rate < min){
                min = rate;
            }
            if (rate > max){
                max = rate;
            }
            if (rate != 0){
                average += rate;
                count += 1;
            }
        }
        std::cout << "Min " << (100 * min) << " Max " << (100 * max) << " Average " << (100 * average / count) << " Count " << count << " Length " << length() << " Rule rate " << (100.0 * (double)count / (double) length()) << std::endl;
    }

    char get(const int position){
        if (position >= max || position < 0){
            return '\0';
        }

        // std::cout << "Read char '" << buffer[position] << "'" << std::endl;

        return buffer[position];
        /*
        char z;
        stream.seekg(position, std::ios_base::beg);
        stream >> z;
        return z;
        */
    }

    bool find(const char * str, const int position){
        if (position >= max || position < 0){
            return false;
        }
        return strncmp(&buffer[position], str, max - position) == 0;
    }

    void growMemo(){
        int newSize = memo_size * 2;
        Column ** newMemo = new Column*[newSize];
        memcpy(newMemo, memo, sizeof(Column*) * memo_size);
        memset(&newMemo[memo_size], 0, sizeof(Column*) * (newSize - memo_size));
        /*
        for (int i = memo_size; i < newSize; i++){
            newMemo[i] = new Column();
        }
        */
        delete[] memo;
        memo = newMemo;
        memo_size = newSize;
    }

    /* I'm sure this can be optimized. It only takes into account
     * the last position used to get line information rather than
     * finding a position closest to the one asked for.
     * So if the last position is 20 and the current position being requested
     * is 15 then this function will compute the information starting from 0.
     * If the information for 10 was computed then that should be used instead.
     * Maybe something like, sort the positions, find closest match lower
     * than the position and start from there.
     */
    LineInfo makeLineInfo(int last_line_position, int position){
        int line = line_info[last_line_position].line;
        int column = line_info[last_line_position].column;
        for (int i = last_line_position + 1; i < position; i++){
            if (buffer[i] == '\n'){
                line += 1;
                column = 1;
            } else {
                column += 1;
            }
        }
        return LineInfo(line, column);
    }

    void updateLineInfo(int position){
        if (line_info.find(position) == line_info.end()){
            if (position > last_line_info){
                line_info[position] = makeLineInfo(last_line_info, position);
            } else {
                line_info[position] = makeLineInfo(0, position);
            }
            last_line_info = position;
        }
    }

    const LineInfo & getLineInfo(int position){
        updateLineInfo(position);
        return line_info[position];
    }

    /* throws a ParseException */
    void reportError(const std::string & parsingContext){
        std::ostringstream out;
        int line = 1;
        int column = 1;
        for (int i = 0; i < farthest; i++){
            if (buffer[i] == '\n'){
                line += 1;
                column = 1;
            } else {
                column += 1;
            }
        }
        int context = 15;
        int left = farthest - context;
        int right = farthest + context;
        if (left < 0){
            left = 0;
        }
        if (right >= max){
            right = max;
        }
        out << "Error while parsing " << parsingContext << ". Read up till line " << line << " column " << column << std::endl;
        std::ostringstream show;
        for (int i = left; i < right; i++){
            char c = buffer[i];
            switch (buffer[i]){
                case '\n' : {
                    show << '\\';
                    show << 'n';
                    break;
                }
                case '\r' : {
                    show << '\\';
                    show << 'r';
                    break;
                }
                case '\t' : {
                    show << '\\';
                    show << 't';
                    break;
                }
                default : show << c; break;
            }
        }
        out << "'" << show.str() << "'" << std::endl;
        for (int i = 0; i < farthest - left; i++){
            out << " ";
        }
        out << "^" << std::endl;
        out << "Last successful rule trace" << std::endl;
        out << makeBacktrace() << std::endl;
        throw ParseException(out.str(), line, column);
    }

    std::string makeBacktrace(){
        std::ostringstream out;

        bool first = true;
        for (std::vector<std::string>::iterator it = last_trace.begin(); it != last_trace.end(); it++){
            if (!first){
                out << " -> ";
            } else {
                first = false;
            }
            out << *it;
        }

        return out.str();
    }

    inline Column & getColumn(const int position){
        while (position >= memo_size){
            growMemo();
        }
        /* create columns lazily because not every position will have a column. */
        if (memo[position] == NULL){
            memo[position] = new Column();
        }
        return *(memo[position]);
    }

    void update(const int position){
        if (position > farthest){
            farthest = position;
            last_trace = rule_backtrace;
        }
    }

    void push_rule(const char * name){
        rule_backtrace.push_back(name);
    }

    void pop_rule(){
        rule_backtrace.pop_back();
    }

    ~Stream(){
        delete[] temp;
        for (int i = 0; i < memo_size; i++){
            delete memo[i];
        }
        delete[] memo;
    }

private:
    char * temp;
    const char * buffer;
    /* an array is faster and uses less memory than std::map */
    Column ** memo;
    int memo_size;
    int max;
    int farthest;
    std::vector<std::string> rule_backtrace;
    std::vector<std::string> last_trace;
    int last_line_info;
    std::map<int, LineInfo> line_info;
};

static int getCurrentLine(const Value & value){
    Stream::LineInfo * info = (Stream::LineInfo*) value.getValue();
    return info->line;
}

static int getCurrentColumn(const Value & value){
    Stream::LineInfo * info = (Stream::LineInfo*) value.getValue();
    return info->column;
}

class RuleTrace{
public:
    RuleTrace(Stream & stream, const char * name):
    stream(stream){
        stream.push_rule(name);
    }

    ~RuleTrace(){
        stream.pop_rule();
    }

    Stream & stream;
};

static inline bool compareChar(const char a, const char b){
    return a == b;
}

static inline char lower(const char x){
    if (x >= 'A' && x <= 'Z'){
        return x - 'A' + 'a';
    }
    return x;
}

static inline bool compareCharCase(const char a, const char b){
    return lower(a) == lower(b);
}


std::string ParseException::getReason() const {
return message;
}

int ParseException::getLine() const {
return line;
}

int ParseException::getColumn() const {
return column;
}

Result errorResult(-1);

Result rule_Identifier(Stream &, const int);
Result rule_IdentCont(Stream &, const int);
Result rule_File(Stream &, const int);
Result rule_Typedef(Stream &, const int);
Result rule_Verbatim(Stream &, const int);
Result rule_RealType(Stream &, const int);
Result rule_String(Stream &, const int);
Result rule_Things(Stream &, const int);
Result rule_Include(Stream &, const int);
Result rule_FirstParam(Stream &, const int);
Result rule_LastParam(Stream &, const int);
Result rule_Parameters(Stream &, const int);
Result rule_Rpc(Stream &, const int);
Result rule_Message(Stream &, const int);
Result rule_Message_field(Stream &, const int);
Result rule_Projection(Stream &, const int);
Result rule_Proj_type(Stream &, const int);
Result rule_Start(Stream &, const int);
Result rule_Specification(Stream &, const int);
Result rule_ProjField(Stream &, const int);
Result rule_Last(Stream &, const int);
Result rule_Declaration(Stream &, const int);
Result rule_Comment(Stream &, const int);
Result rule_Comment_rest(Stream &, const int);
Result rule_Line_comm_rest(Stream &, const int);
Result rule_Line_comment(Stream &, const int);
Result rule_Simple_Type(Stream &, const int);
Result rule_Type_w_modifier(Stream &, const int);
Result rule_Type(Stream &, const int);
Result rule_Type_form(Stream &, const int);
Result rule_Spacing(Stream &, const int);
Result rule_Space(Stream &, const int);




Result rule_Identifier(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_Identifier.calculated()){
        return column_peg_1.chunk0->chunk_Identifier;
    }
    
    RuleTrace trace_peg_15(stream, "Identifier");
    int myposition = position;
    
    
    Value i1;
        Value i2;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Identifier at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: i1:IdentStart i2:IdentCont* Spacing {{ \n			std::ostringstream total;\n			total << (char) (intptr_t) i1.getValue();\n\n		   	for(Value::iterator it = i2.getValues().begin(); it != i2.getValues().end(); it ++)\n		   	{\n				const Value & v = *it;\n				char letter = (char) (intptr_t) v.getValue();\n				total << letter;\n		   	}\n				std::string str = total.str();\n		   		value = &str[0]; }}" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    
                    char letter_peg_9 = stream.get(result_peg_2.getPosition());
                    if (letter_peg_9 != '\0' && strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", letter_peg_9) != NULL){
                        result_peg_2.nextPosition();
                        result_peg_2.setValue(Value((void*) (long) letter_peg_9));
                    } else {
                        result_peg_2.setPosition(position_peg_6);
                        goto out_peg_8;
                    }
                    
                }
                goto success_peg_4;
                out_peg_8:
                goto out_peg_10;
                success_peg_4:
                ;
                i1 = result_peg_2.getValues();
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_13(result_peg_2.getPosition());
                    result_peg_13 = rule_IdentCont(stream, result_peg_13.getPosition());
                    if (result_peg_13.error()){
                        goto loop_peg_12;
                    }
                    result_peg_2.addResult(result_peg_13);
                } while (true);
                loop_peg_12:
                ;
                i2 = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_10;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			total << (char) (intptr_t) i1.getValue();
                    
                    		   	for(Value::iterator it = i2.getValues().begin(); it != i2.getValues().end(); it ++)
                    		   	{
                    				const Value & v = *it;
                    				char letter = (char) (intptr_t) v.getValue();
                    				total << letter;
                    		   	}
                    				std::string str = total.str();
                    		   		value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Identifier = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Identifier at position " << result_peg_2.getPosition() << " alternative: i1:IdentStart i2:IdentCont* Spacing {{ \n			std::ostringstream total;\n			total << (char) (intptr_t) i1.getValue();\n\n		   	for(Value::iterator it = i2.getValues().begin(); it != i2.getValues().end(); it ++)\n		   	{\n				const Value & v = *it;\n				char letter = (char) (intptr_t) v.getValue();\n				total << letter;\n		   	}\n				std::string str = total.str();\n		   		value = &str[0]; }}" << std::endl;
        return result_peg_2;
        out_peg_10:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Identifier = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_IdentCont(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_IdentCont.calculated()){
        return column_peg_1.chunk0->chunk_IdentCont;
    }
    
    RuleTrace trace_peg_13(stream, "IdentCont");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule IdentCont at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: IdentStart" << std::endl;
        {
            int position_peg_5 = result_peg_2.getPosition();
            
            char letter_peg_8 = stream.get(result_peg_2.getPosition());
            if (letter_peg_8 != '\0' && strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", letter_peg_8) != NULL){
                result_peg_2.nextPosition();
                result_peg_2.setValue(Value((void*) (long) letter_peg_8));
            } else {
                result_peg_2.setPosition(position_peg_5);
                goto out_peg_7;
            }
            
        }
        goto success_peg_3;
        out_peg_7:
        goto out_peg_9;
        success_peg_3:
        ;
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_IdentCont = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule IdentCont at position " << result_peg_2.getPosition() << " alternative: IdentStart" << std::endl;
        return result_peg_2;
        out_peg_9:
        Result result_peg_10(myposition);
        std::cout << "Trying rule IdentCont at " << myposition << " '" << stream.get(result_peg_10.getPosition()) << "' alternative: [0123456789]" << std::endl;
        char letter_peg_11 = stream.get(result_peg_10.getPosition());
        if (letter_peg_11 != '\0' && strchr("0123456789", letter_peg_11) != NULL){
            result_peg_10.nextPosition();
            result_peg_10.setValue(Value((void*) (long) letter_peg_11));
        } else {
            goto out_peg_12;
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_IdentCont = result_peg_10;
        stream.update(result_peg_10.getPosition());
        
        std::cout << "Succeeded rule IdentCont at position " << result_peg_10.getPosition() << " alternative: [0123456789]" << std::endl;
        return result_peg_10;
        out_peg_12:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_IdentCont = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_File(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_File.calculated()){
        return column_peg_1.chunk0->chunk_File;
    }
    
    RuleTrace trace_peg_11(stream, "File");
    int myposition = position;
    
    
    Value v;
        Value t;
    Result result_peg_2(myposition);
        std::cout << "Trying rule File at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Spacing v:Verbatim? t:Things* <eof> {{ std::vector<Definition *>* defs = new std::vector<Definition *>;\n			std::vector<Rpc *>* rpcs = new std::vector<Rpc *>;\n			std::vector<Message *>* messages = new std::vector<Message *>;\n			std::map<char *, Definition*> *env = new std::map<char *, Definition *>;\n			for(Value::iterator it = t.getValues().begin(); it != t.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				Definition * ds = (Definition *) v.getValue();\n				if(!ds)\n				{\n					printf(\"error null pointer\\n\");\n					exit(0);\n				}\n				switch (ds->get_definition_type())\n				{\n				case kRpc:\n				{\n				Rpc * rp = reinterpret_cast<Rpc *>(ds);\n				rpcs->push_back(rp);\n				}				\n				break;\n				case kMessage:\n				{\n				Message * mess = reinterpret_cast<Message *>(ds);\n				messages->push_back(mess);\n				}\n				break;\n				case kProjection:\n				{\n				Projection* proj = reinterpret_cast<Projection *>(ds);\n				env->insert(std::pair<char *, Definition *>(proj->name(), ds));\n				}\n				case kTypedef:\n				{\n				Typedef* td = reinterpret_cast<Typedef*>(ds);\n				env->insert(std::pair<char*, Definition*>(td->name(), ds));\n				}\n				break;\n				default:\n				{\n				printf(\"fatal error\\n\");\n				}\n				}\n\n			}\n			  value = new Scope((char*) v.getValue(), rpcs, messages, env); }}" << std::endl;
        {
        
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            int save_peg_6 = result_peg_2.getPosition();
                
                result_peg_2 = rule_Verbatim(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    
                    result_peg_2 = Result(save_peg_6);
                    result_peg_2.setValue(Value((void*) 0));
                    
                }
                v = result_peg_2.getValues();
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_9(result_peg_2.getPosition());
                    result_peg_9 = rule_Things(stream, result_peg_9.getPosition());
                    if (result_peg_9.error()){
                        goto loop_peg_8;
                    }
                    result_peg_2.addResult(result_peg_9);
                } while (true);
                loop_peg_8:
                ;
                t = result_peg_2.getValues();
            
            
            
            if ('\0' == stream.get(result_peg_2.getPosition())){
                    result_peg_2.nextPosition();
                    result_peg_2.setValue(Value((void *) '\0'));
                } else {
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::vector<Definition *>* defs = new std::vector<Definition *>;
                    			std::vector<Rpc *>* rpcs = new std::vector<Rpc *>;
                    			std::vector<Message *>* messages = new std::vector<Message *>;
                    			std::map<char *, Definition*> *env = new std::map<char *, Definition *>;
                    			for(Value::iterator it = t.getValues().begin(); it != t.getValues().end(); it ++)
                    			{
                    				const Value & v = *it;
                    				Definition * ds = (Definition *) v.getValue();
                    				if(!ds)
                    				{
                    					printf("error null pointer\n");
                    					exit(0);
                    				}
                    				switch (ds->get_definition_type())
                    				{
                    				case kRpc:
                    				{
                    				Rpc * rp = reinterpret_cast<Rpc *>(ds);
                    				rpcs->push_back(rp);
                    				}				
                    				break;
                    				case kMessage:
                    				{
                    				Message * mess = reinterpret_cast<Message *>(ds);
                    				messages->push_back(mess);
                    				}
                    				break;
                    				case kProjection:
                    				{
                    				Projection* proj = reinterpret_cast<Projection *>(ds);
                    				env->insert(std::pair<char *, Definition *>(proj->name(), ds));
                    				}
                    				case kTypedef:
                    				{
                    				Typedef* td = reinterpret_cast<Typedef*>(ds);
                    				env->insert(std::pair<char*, Definition*>(td->name(), ds));
                    				}
                    				break;
                    				default:
                    				{
                    				printf("fatal error\n");
                    				}
                    				}
                    
                    			}
                    			  value = new Scope((char*) v.getValue(), rpcs, messages, env);
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_File = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule File at position " << result_peg_2.getPosition() << " alternative: Spacing v:Verbatim? t:Things* <eof> {{ std::vector<Definition *>* defs = new std::vector<Definition *>;\n			std::vector<Rpc *>* rpcs = new std::vector<Rpc *>;\n			std::vector<Message *>* messages = new std::vector<Message *>;\n			std::map<char *, Definition*> *env = new std::map<char *, Definition *>;\n			for(Value::iterator it = t.getValues().begin(); it != t.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				Definition * ds = (Definition *) v.getValue();\n				if(!ds)\n				{\n					printf(\"error null pointer\\n\");\n					exit(0);\n				}\n				switch (ds->get_definition_type())\n				{\n				case kRpc:\n				{\n				Rpc * rp = reinterpret_cast<Rpc *>(ds);\n				rpcs->push_back(rp);\n				}				\n				break;\n				case kMessage:\n				{\n				Message * mess = reinterpret_cast<Message *>(ds);\n				messages->push_back(mess);\n				}\n				break;\n				case kProjection:\n				{\n				Projection* proj = reinterpret_cast<Projection *>(ds);\n				env->insert(std::pair<char *, Definition *>(proj->name(), ds));\n				}\n				case kTypedef:\n				{\n				Typedef* td = reinterpret_cast<Typedef*>(ds);\n				env->insert(std::pair<char*, Definition*>(td->name(), ds));\n				}\n				break;\n				default:\n				{\n				printf(\"fatal error\\n\");\n				}\n				}\n\n			}\n			  value = new Scope((char*) v.getValue(), rpcs, messages, env); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_File = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Typedef(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_Typedef.calculated()){
        return column_peg_1.chunk0->chunk_Typedef;
    }
    
    RuleTrace trace_peg_18(stream, "Typedef");
    int myposition = position;
    
    
    Value rt;
        Value mt;
        Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Typedef at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"typedef\" Spacing \"<\" Spacing rt:RealType Spacing \",\" Spacing mt:Type Spacing \">\" Spacing id:Identifier Spacing {{ \n			 value = new Typedef((char*) rt.getValue(), (Type*) mt.getValue(), (char*) id.getValue());}}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "typedef"));
                for (int i = 0; i < 7; i++){
                    if (compareChar("typedef"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) "<"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("<"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_RealType(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                rt = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) ","));
                for (int i = 0; i < 1; i++){
                    if (compareChar(","[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Type(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                mt = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) ">"));
                for (int i = 0; i < 1; i++){
                    if (compareChar(">"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new Typedef((char*) rt.getValue(), (Type*) mt.getValue(), (char*) id.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Typedef = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Typedef at position " << result_peg_2.getPosition() << " alternative: \"typedef\" Spacing \"<\" Spacing rt:RealType Spacing \",\" Spacing mt:Type Spacing \">\" Spacing id:Identifier Spacing {{ \n			 value = new Typedef((char*) rt.getValue(), (Type*) mt.getValue(), (char*) id.getValue());}}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Typedef = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Verbatim(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_Verbatim.calculated()){
        return column_peg_1.chunk0->chunk_Verbatim;
    }
    
    RuleTrace trace_peg_21(stream, "Verbatim");
    int myposition = position;
    
    
    Value contents;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Verbatim at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"verbatim\" Spacing \"[\" contents:VerbatimContent \"]\" {{ std::ostringstream total;\n			for(Value::iterator it = contents.getValues().begin(); it != contents.getValues().end(); it ++)\n			{\n			const Value &v = *it;\n			char letter = (char) (intptr_t) v.getValue();\n			total << letter;     \n			}\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "verbatim"));
                for (int i = 0; i < 8; i++){
                    if (compareChar("verbatim"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) "["));
                for (int i = 0; i < 1; i++){
                    if (compareChar("["[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            {
                    int position_peg_10 = result_peg_2.getPosition();
                    {
                    
                        Result result_peg_18(result_peg_2);
                            result_peg_18.setValue(Value((void*) "]"));
                            for (int i = 0; i < 1; i++){
                                if (compareChar("]"[i], stream.get(result_peg_18.getPosition()))){
                                    result_peg_18.nextPosition();
                                } else {
                                    goto not_peg_17;
                                }
                            }
                            result_peg_2.setPosition(position_peg_10);
                            goto out_peg_14;
                            not_peg_17:
                            result_peg_2.setValue(Value((void*)0));
                        
                        
                        
                        char temp_peg_19 = stream.get(result_peg_2.getPosition());
                            if (temp_peg_19 != '\0'){
                                result_peg_2.setValue(Value((void*) (long) temp_peg_19));
                                result_peg_2.nextPosition();
                            } else {
                                result_peg_2.setPosition(position_peg_10);
                                goto out_peg_14;
                            }
                        
                        
                    }
                }
                goto success_peg_8;
                out_peg_14:
                goto out_peg_4;
                success_peg_8:
                ;
                contents = result_peg_2.getValues();
            
            
            
            result_peg_2.setValue(Value((void*) "]"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("]"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			for(Value::iterator it = contents.getValues().begin(); it != contents.getValues().end(); it ++)
                    			{
                    			const Value &v = *it;
                    			char letter = (char) (intptr_t) v.getValue();
                    			total << letter;     
                    			}
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Verbatim = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Verbatim at position " << result_peg_2.getPosition() << " alternative: \"verbatim\" Spacing \"[\" contents:VerbatimContent \"]\" {{ std::ostringstream total;\n			for(Value::iterator it = contents.getValues().begin(); it != contents.getValues().end(); it ++)\n			{\n			const Value &v = *it;\n			char letter = (char) (intptr_t) v.getValue();\n			total << letter;     \n			}\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_Verbatim = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_RealType(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_RealType.calculated()){
        return column_peg_1.chunk1->chunk_RealType;
    }
    
    RuleTrace trace_peg_8(stream, "RealType");
    int myposition = position;
    
    
    Value fp;
        Value sp;
    Result result_peg_2(myposition);
        std::cout << "Trying rule RealType at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: fp:Identifier? Spacing sp:Identifier {{ //temporary solution\n			std::ostringstream total;\n			total << fp.getValue() << ' ' << sp.getValue();\n			std::string str = total.str();\n			value = &str[0];\n			}}" << std::endl;
        {
        
            int save_peg_4 = result_peg_2.getPosition();
                
                result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    
                    result_peg_2 = Result(save_peg_4);
                    result_peg_2.setValue(Value((void*) 0));
                    
                }
                fp = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_6;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_6;
                }
                sp = result_peg_2.getValues();
            
            
            
            {
                    Value value((void*) 0);
                    //temporary solution
                    			std::ostringstream total;
                    			total << fp.getValue() << ' ' << sp.getValue();
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_RealType = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule RealType at position " << result_peg_2.getPosition() << " alternative: fp:Identifier? Spacing sp:Identifier {{ //temporary solution\n			std::ostringstream total;\n			total << fp.getValue() << ' ' << sp.getValue();\n			std::string str = total.str();\n			value = &str[0];\n			}}" << std::endl;
        return result_peg_2;
        out_peg_6:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_RealType = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_String(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_String.calculated()){
        return column_peg_1.chunk1->chunk_String;
    }
    
    RuleTrace trace_peg_15(stream, "String");
    int myposition = position;
    
    
    Value l;
    Result result_peg_2(myposition);
        std::cout << "Trying rule String at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: <ascii 34> l:any_letter+ <ascii 34> {{\n			std::ostringstream total;\n			for(Value::iterator it = l.getValues().begin(); it != l.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				char letter = (char) (intptr_t) v.getValue();\n				\n				if(letter == '\\')\n				{\n					if((it++) == l.getValues().end())\n					{\n						total << letter;\n						break;\n					}	\n					\n					letter = (char) (intptr_t) (*it).getValue();\n					switch (letter)\n					{\n					case 'n':\n					     total <<'\\n';\n					     break;\n					case 't':\n					     total << '\\t';\n					     break;\n					default:\n						total << letter;\n					}\n					continue;\n				}\n				total << letter;\n			}\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) 34));
                if ((unsigned char) stream.get(result_peg_2.getPosition()) == (unsigned char) 34){
                    result_peg_2.nextPosition();
                } else {
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    {
                        int position_peg_10 = result_peg_7.getPosition();
                        
                        char letter_peg_13 = stream.get(result_peg_7.getPosition());
                        if (letter_peg_13 != '\0' && strchr("_.,/?<>'; =:%`!@#$^&*()-+{}|\\ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", letter_peg_13) != NULL){
                            result_peg_7.nextPosition();
                            result_peg_7.setValue(Value((void*) (long) letter_peg_13));
                        } else {
                            result_peg_7.setPosition(position_peg_10);
                            goto out_peg_12;
                        }
                        
                    }
                    goto success_peg_8;
                    out_peg_12:
                    goto loop_peg_6;
                    success_peg_8:
                    ;
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
                l = result_peg_2.getValues();
            
            
            
            result_peg_2.setValue(Value((void*) 34));
                if ((unsigned char) stream.get(result_peg_2.getPosition()) == (unsigned char) 34){
                    result_peg_2.nextPosition();
                } else {
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			for(Value::iterator it = l.getValues().begin(); it != l.getValues().end(); it ++)
                    			{
                    				const Value & v = *it;
                    				char letter = (char) (intptr_t) v.getValue();
                    				
                    				if(letter == '\\')
                    				{
                    					if((it++) == l.getValues().end())
                    					{
                    						total << letter;
                    						break;
                    					}	
                    					
                    					letter = (char) (intptr_t) (*it).getValue();
                    					switch (letter)
                    					{
                    					case 'n':
                    					     total <<'\n';
                    					     break;
                    					case 't':
                    					     total << '\t';
                    					     break;
                    					default:
                    						total << letter;
                    					}
                    					continue;
                    				}
                    				total << letter;
                    			}
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_String = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule String at position " << result_peg_2.getPosition() << " alternative: <ascii 34> l:any_letter+ <ascii 34> {{\n			std::ostringstream total;\n			for(Value::iterator it = l.getValues().begin(); it != l.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				char letter = (char) (intptr_t) v.getValue();\n				\n				if(letter == '\\')\n				{\n					if((it++) == l.getValues().end())\n					{\n						total << letter;\n						break;\n					}	\n					\n					letter = (char) (intptr_t) (*it).getValue();\n					switch (letter)\n					{\n					case 'n':\n					     total <<'\\n';\n					     break;\n					case 't':\n					     total << '\\t';\n					     break;\n					default:\n						total << letter;\n					}\n					continue;\n				}\n				total << letter;\n			}\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_String = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Things(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_Things.calculated()){
        return column_peg_1.chunk1->chunk_Things;
    }
    
    RuleTrace trace_peg_10(stream, "Things");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Things at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Typedef" << std::endl;
        result_peg_2 = rule_Typedef(stream, result_peg_2.getPosition());
        if (result_peg_2.error()){
            goto out_peg_3;
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Things = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Things at position " << result_peg_2.getPosition() << " alternative: Typedef" << std::endl;
        return result_peg_2;
        out_peg_3:
        Result result_peg_4(myposition);
        std::cout << "Trying rule Things at " << myposition << " '" << stream.get(result_peg_4.getPosition()) << "' alternative: Rpc" << std::endl;
        result_peg_4 = rule_Rpc(stream, result_peg_4.getPosition());
        if (result_peg_4.error()){
            goto out_peg_5;
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Things = result_peg_4;
        stream.update(result_peg_4.getPosition());
        
        std::cout << "Succeeded rule Things at position " << result_peg_4.getPosition() << " alternative: Rpc" << std::endl;
        return result_peg_4;
        out_peg_5:
        Result result_peg_6(myposition);
        std::cout << "Trying rule Things at " << myposition << " '" << stream.get(result_peg_6.getPosition()) << "' alternative: Message" << std::endl;
        result_peg_6 = rule_Message(stream, result_peg_6.getPosition());
        if (result_peg_6.error()){
            goto out_peg_7;
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Things = result_peg_6;
        stream.update(result_peg_6.getPosition());
        
        std::cout << "Succeeded rule Things at position " << result_peg_6.getPosition() << " alternative: Message" << std::endl;
        return result_peg_6;
        out_peg_7:
        Result result_peg_8(myposition);
        std::cout << "Trying rule Things at " << myposition << " '" << stream.get(result_peg_8.getPosition()) << "' alternative: Projection" << std::endl;
        result_peg_8 = rule_Projection(stream, result_peg_8.getPosition());
        if (result_peg_8.error()){
            goto out_peg_9;
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Things = result_peg_8;
        stream.update(result_peg_8.getPosition());
        
        std::cout << "Succeeded rule Things at position " << result_peg_8.getPosition() << " alternative: Projection" << std::endl;
        return result_peg_8;
        out_peg_9:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Things = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Include(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_Include.calculated()){
        return column_peg_1.chunk1->chunk_Include;
    }
    
    RuleTrace trace_peg_22(stream, "Include");
    int myposition = position;
    
    
    Value f;
        Value s;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Include at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"#include\" Space+ \"<\" Spacing f:Identifier Spacing \">\" Spacing {{\n			std::ostringstream total;\n			total << \"#include <\" << (char *) f.getValue() << \">\";\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "#include"));
                for (int i = 0; i < 8; i++){
                    if (compareChar("#include"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) "<"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("<"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                f = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) ">"));
                for (int i = 0; i < 1; i++){
                    if (compareChar(">"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			total << "#include <" << (char *) f.getValue() << ">";
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Include = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Include at position " << result_peg_2.getPosition() << " alternative: \"#include\" Space+ \"<\" Spacing f:Identifier Spacing \">\" Spacing {{\n			std::ostringstream total;\n			total << \"#include <\" << (char *) f.getValue() << \">\";\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        return result_peg_2;
        out_peg_4:
        Result result_peg_14(myposition);
        std::cout << "Trying rule Include at " << myposition << " '" << stream.get(result_peg_14.getPosition()) << "' alternative: \"#include\" Space+ s:String Spacing {{\n			std::ostringstream total;\n			total << \"#include \" << (char *) s.getValue();\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        {
        
            result_peg_14.setValue(Value((void*) "#include"));
                for (int i = 0; i < 8; i++){
                    if (compareChar("#include"[i], stream.get(result_peg_14.getPosition()))){
                        result_peg_14.nextPosition();
                    } else {
                        goto out_peg_16;
                    }
                }
            
            
            
            result_peg_14.reset();
                do{
                    Result result_peg_19(result_peg_14.getPosition());
                    result_peg_19 = rule_Space(stream, result_peg_19.getPosition());
                    if (result_peg_19.error()){
                        goto loop_peg_18;
                    }
                    result_peg_14.addResult(result_peg_19);
                } while (true);
                loop_peg_18:
                if (result_peg_14.matches() == 0){
                    goto out_peg_16;
                }
            
            
            
            result_peg_14 = rule_String(stream, result_peg_14.getPosition());
                if (result_peg_14.error()){
                    goto out_peg_16;
                }
                s = result_peg_14.getValues();
            
            
            
            result_peg_14 = rule_Spacing(stream, result_peg_14.getPosition());
                if (result_peg_14.error()){
                    goto out_peg_16;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			total << "#include " << (char *) s.getValue();
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_14.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Include = result_peg_14;
        stream.update(result_peg_14.getPosition());
        
        std::cout << "Succeeded rule Include at position " << result_peg_14.getPosition() << " alternative: \"#include\" Space+ s:String Spacing {{\n			std::ostringstream total;\n			total << \"#include \" << (char *) s.getValue();\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        return result_peg_14;
        out_peg_16:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_Include = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_FirstParam(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_FirstParam.calculated()){
        return column_peg_1.chunk1->chunk_FirstParam;
    }
    
    RuleTrace trace_peg_13(stream, "FirstParam");
    int myposition = position;
    
    
    Value t;
        Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule FirstParam at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: t:Type_form id:Identifier Spacing COMMA Spacing {{\n			value = new Parameter((Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        {
        
            result_peg_2 = rule_Type_form(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_10 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ","));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(","[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_10);
                            goto out_peg_11;
                        }
                    }
                        
                }
                goto success_peg_8;
                out_peg_11:
                goto out_peg_4;
                success_peg_8:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new Parameter((Type *) t.getValue(), (char *) id.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_FirstParam = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule FirstParam at position " << result_peg_2.getPosition() << " alternative: t:Type_form id:Identifier Spacing COMMA Spacing {{\n			value = new Parameter((Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_FirstParam = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_LastParam(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk2 != 0 && column_peg_1.chunk2->chunk_LastParam.calculated()){
        return column_peg_1.chunk2->chunk_LastParam;
    }
    
    RuleTrace trace_peg_7(stream, "LastParam");
    int myposition = position;
    
    
    Value t;
        Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule LastParam at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: t:Type_form id:Identifier Spacing {{\n			value = new Parameter((Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        {
        
            result_peg_2 = rule_Type_form(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new Parameter((Type *) t.getValue(), (char *) id.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_LastParam = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule LastParam at position " << result_peg_2.getPosition() << " alternative: t:Type_form id:Identifier Spacing {{\n			value = new Parameter((Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_LastParam = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Parameters(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk2 != 0 && column_peg_1.chunk2->chunk_Parameters.calculated()){
        return column_peg_1.chunk2->chunk_Parameters;
    }
    
    RuleTrace trace_peg_8(stream, "Parameters");
    int myposition = position;
    
    
    Value f;
        Value l;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Parameters at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: f:FirstParam* l:LastParam {{\n			std::vector<Parameter* >* params = new std::vector<Parameter* >;\n			for(Value::iterator it = f.getValues().begin(); it != f.getValues().end(); it ++)\n			{ \n				const Value & v = *it;\n				Parameter* p = (Parameter* ) v.getValue();\n				params->push_back(p);\n			}\n			params->push_back((Parameter* ) l.getValue());\n			value = params; }}" << std::endl;
        {
        
            result_peg_2.reset();
                do{
                    Result result_peg_5(result_peg_2.getPosition());
                    result_peg_5 = rule_FirstParam(stream, result_peg_5.getPosition());
                    if (result_peg_5.error()){
                        goto loop_peg_4;
                    }
                    result_peg_2.addResult(result_peg_5);
                } while (true);
                loop_peg_4:
                ;
                f = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_LastParam(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_7;
                }
                l = result_peg_2.getValues();
            
            
            
            {
                    Value value((void*) 0);
                    std::vector<Parameter* >* params = new std::vector<Parameter* >;
                    			for(Value::iterator it = f.getValues().begin(); it != f.getValues().end(); it ++)
                    			{ 
                    				const Value & v = *it;
                    				Parameter* p = (Parameter* ) v.getValue();
                    				params->push_back(p);
                    			}
                    			params->push_back((Parameter* ) l.getValue());
                    			value = params;
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Parameters = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Parameters at position " << result_peg_2.getPosition() << " alternative: f:FirstParam* l:LastParam {{\n			std::vector<Parameter* >* params = new std::vector<Parameter* >;\n			for(Value::iterator it = f.getValues().begin(); it != f.getValues().end(); it ++)\n			{ \n				const Value & v = *it;\n				Parameter* p = (Parameter* ) v.getValue();\n				params->push_back(p);\n			}\n			params->push_back((Parameter* ) l.getValue());\n			value = params; }}" << std::endl;
        return result_peg_2;
        out_peg_7:
    
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Parameters = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Rpc(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk2 != 0 && column_peg_1.chunk2->chunk_Rpc.calculated()){
        return column_peg_1.chunk2->chunk_Rpc;
    }
    
    RuleTrace trace_peg_28(stream, "Rpc");
    int myposition = position;
    
    
    Value t;
        Value id;
        Value p;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Rpc at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"rpc\" Space+ t:Type_form id:Identifier Spacing OPEN p:Parameters CLOSE SEMI Spacing {{\n			value = new Rpc((Type *) t.getValue(), (char *) id.getValue(), (std::vector<Parameter *> *) p.getValue()); }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "rpc"));
                for (int i = 0; i < 3; i++){
                    if (compareChar("rpc"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Type_form(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_14 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "("));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("("[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_14);
                            goto out_peg_15;
                        }
                    }
                        
                }
                goto success_peg_12;
                out_peg_15:
                goto out_peg_4;
                success_peg_12:
                ;
            
            
            
            result_peg_2 = rule_Parameters(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                p = result_peg_2.getValues();
            
            
            
            {
                    int position_peg_20 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ")"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(")"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_20);
                            goto out_peg_21;
                        }
                    }
                        
                }
                goto success_peg_18;
                out_peg_21:
                goto out_peg_4;
                success_peg_18:
                ;
            
            
            
            {
                    int position_peg_25 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ";"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(";"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_25);
                            goto out_peg_26;
                        }
                    }
                        
                }
                goto success_peg_23;
                out_peg_26:
                goto out_peg_4;
                success_peg_23:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new Rpc((Type *) t.getValue(), (char *) id.getValue(), (std::vector<Parameter *> *) p.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Rpc = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Rpc at position " << result_peg_2.getPosition() << " alternative: \"rpc\" Space+ t:Type_form id:Identifier Spacing OPEN p:Parameters CLOSE SEMI Spacing {{\n			value = new Rpc((Type *) t.getValue(), (char *) id.getValue(), (std::vector<Parameter *> *) p.getValue()); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Rpc = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Message(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk2 != 0 && column_peg_1.chunk2->chunk_Message.calculated()){
        return column_peg_1.chunk2->chunk_Message;
    }
    
    RuleTrace trace_peg_25(stream, "Message");
    int myposition = position;
    
    
    Value id;
        Value m;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Message at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"message\" Space+ id:Identifier Spacing OPENC Spacing m:Message_field* CLOSEC Spacing {{ std::vector<MessageField* >* fields = new std::vector<MessageField* >;\n			for(Value::iterator it = m.getValues().begin(); it != m.getValues().end(); it ++)\n			{		   \n				const Value & v = *it;\n				MessageField* mess_field = (MessageField *) v.getValue();\n				fields->push_back(mess_field);\n			}\n			value = new Message((char *) id.getValue(), fields); }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "message"));
                for (int i = 0; i < 7; i++){
                    if (compareChar("message"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_13 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "{"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("{"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_13);
                            goto out_peg_14;
                        }
                    }
                        
                }
                goto success_peg_11;
                out_peg_14:
                goto out_peg_4;
                success_peg_11:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_18(result_peg_2.getPosition());
                    result_peg_18 = rule_Message_field(stream, result_peg_18.getPosition());
                    if (result_peg_18.error()){
                        goto loop_peg_17;
                    }
                    result_peg_2.addResult(result_peg_18);
                } while (true);
                loop_peg_17:
                ;
                m = result_peg_2.getValues();
            
            
            
            {
                    int position_peg_22 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "}"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("}"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_22);
                            goto out_peg_23;
                        }
                    }
                        
                }
                goto success_peg_20;
                out_peg_23:
                goto out_peg_4;
                success_peg_20:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::vector<MessageField* >* fields = new std::vector<MessageField* >;
                    			for(Value::iterator it = m.getValues().begin(); it != m.getValues().end(); it ++)
                    			{		   
                    				const Value & v = *it;
                    				MessageField* mess_field = (MessageField *) v.getValue();
                    				fields->push_back(mess_field);
                    			}
                    			value = new Message((char *) id.getValue(), fields);
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Message = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Message at position " << result_peg_2.getPosition() << " alternative: \"message\" Space+ id:Identifier Spacing OPENC Spacing m:Message_field* CLOSEC Spacing {{ std::vector<MessageField* >* fields = new std::vector<MessageField* >;\n			for(Value::iterator it = m.getValues().begin(); it != m.getValues().end(); it ++)\n			{		   \n				const Value & v = *it;\n				MessageField* mess_field = (MessageField *) v.getValue();\n				fields->push_back(mess_field);\n			}\n			value = new Message((char *) id.getValue(), fields); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Message = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Message_field(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk2 != 0 && column_peg_1.chunk2->chunk_Message_field.calculated()){
        return column_peg_1.chunk2->chunk_Message_field;
    }
    
    RuleTrace trace_peg_16(stream, "Message_field");
    int myposition = position;
    
    
    Value t;
        Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Message_field at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: t:Type_form Space+ id:Identifier Spacing SEMI Spacing {{\n			value = new MessageField((Type* ) t.getValue(), (char* ) id.getValue()); }}" << std::endl;
        {
        
            result_peg_2 = rule_Type_form(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_13 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ";"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(";"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_13);
                            goto out_peg_14;
                        }
                    }
                        
                }
                goto success_peg_11;
                out_peg_14:
                goto out_peg_4;
                success_peg_11:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new MessageField((Type* ) t.getValue(), (char* ) id.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Message_field = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Message_field at position " << result_peg_2.getPosition() << " alternative: t:Type_form Space+ id:Identifier Spacing SEMI Spacing {{\n			value = new MessageField((Type* ) t.getValue(), (char* ) id.getValue()); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk2 == 0){
            column_peg_1.chunk2 = new Chunk2();
        }
        column_peg_1.chunk2->chunk_Message_field = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Projection(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk3 != 0 && column_peg_1.chunk3->chunk_Projection.calculated()){
        return column_peg_1.chunk3->chunk_Projection;
    }
    
    RuleTrace trace_peg_29(stream, "Projection");
    int myposition = position;
    
    
    Value id;
        Value t;
        Value p;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Projection at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"projection\" Space+ id:Identifier \"<\" Spacing t:Proj_type \">\" Spacing OPENC Spacing p:ProjField+ CLOSEC Spacing {{ std::vector<ProjectionField *>* p_fields = new std::vector<ProjectionField *>;\n			for(Value::iterator it = p.getValues().begin(); it != p.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				ProjectionField * p = (ProjectionField *) v.getValue();\n				p_fields->push_back(p);\n			} \n			value = new Projection((char* ) id.getValue(), (char* ) t.getValue(), p_fields); }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "projection"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("projection"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2.setValue(Value((void*) "<"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("<"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Proj_type(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2.setValue(Value((void*) ">"));
                for (int i = 0; i < 1; i++){
                    if (compareChar(">"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_17 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "{"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("{"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_17);
                            goto out_peg_18;
                        }
                    }
                        
                }
                goto success_peg_15;
                out_peg_18:
                goto out_peg_4;
                success_peg_15:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_22(result_peg_2.getPosition());
                    result_peg_22 = rule_ProjField(stream, result_peg_22.getPosition());
                    if (result_peg_22.error()){
                        goto loop_peg_21;
                    }
                    result_peg_2.addResult(result_peg_22);
                } while (true);
                loop_peg_21:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
                p = result_peg_2.getValues();
            
            
            
            {
                    int position_peg_26 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "}"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("}"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_26);
                            goto out_peg_27;
                        }
                    }
                        
                }
                goto success_peg_24;
                out_peg_27:
                goto out_peg_4;
                success_peg_24:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::vector<ProjectionField *>* p_fields = new std::vector<ProjectionField *>;
                    			for(Value::iterator it = p.getValues().begin(); it != p.getValues().end(); it ++)
                    			{
                    				const Value & v = *it;
                    				ProjectionField * p = (ProjectionField *) v.getValue();
                    				p_fields->push_back(p);
                    			} 
                    			value = new Projection((char* ) id.getValue(), (char* ) t.getValue(), p_fields);
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Projection = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Projection at position " << result_peg_2.getPosition() << " alternative: \"projection\" Space+ id:Identifier \"<\" Spacing t:Proj_type \">\" Spacing OPENC Spacing p:ProjField+ CLOSEC Spacing {{ std::vector<ProjectionField *>* p_fields = new std::vector<ProjectionField *>;\n			for(Value::iterator it = p.getValues().begin(); it != p.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				ProjectionField * p = (ProjectionField *) v.getValue();\n				p_fields->push_back(p);\n			} \n			value = new Projection((char* ) id.getValue(), (char* ) t.getValue(), p_fields); }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Projection = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Proj_type(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk3 != 0 && column_peg_1.chunk3->chunk_Proj_type.calculated()){
        return column_peg_1.chunk3->chunk_Proj_type;
    }
    
    RuleTrace trace_peg_10(stream, "Proj_type");
    int myposition = position;
    
    
    Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Proj_type at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"struct\" Space+ id:Identifier Spacing {{\n			std::ostringstream total;\n			total << \"struct \" << (char *) id.getValue();\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "struct"));
                for (int i = 0; i < 6; i++){
                    if (compareChar("struct"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                id = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    std::ostringstream total;
                    			total << "struct " << (char *) id.getValue();
                    			std::string str = total.str();
                    			value = &str[0];
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Proj_type = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Proj_type at position " << result_peg_2.getPosition() << " alternative: \"struct\" Space+ id:Identifier Spacing {{\n			std::ostringstream total;\n			total << \"struct \" << (char *) id.getValue();\n			std::string str = total.str();\n			value = &str[0]; }}" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Proj_type = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Start(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk3 != 0 && column_peg_1.chunk3->chunk_Start.calculated()){
        return column_peg_1.chunk3->chunk_Start;
    }
    
    RuleTrace trace_peg_33(stream, "Start");
    int myposition = position;
    
    
    Value k;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Start at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: k:Keyword Spacing COMMA Spacing {{ value = k; }}" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "out"));
                            for (int i = 0; i < 3; i++){
                                if (compareChar("out"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_6);
                                    goto out_peg_8;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			      *x = 1;
                                			      value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_8:
                {
                    int position_peg_11 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "in"));
                            for (int i = 0; i < 2; i++){
                                if (compareChar("in"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_11);
                                    goto out_peg_13;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			       *x = 2; 
                                			       value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_13:
                {
                    int position_peg_16 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "bind"));
                            for (int i = 0; i < 4; i++){
                                if (compareChar("bind"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_16);
                                    goto out_peg_18;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			  	 *x = 3; 
                                			  	 value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_18:
                {
                    int position_peg_21 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "alloc"));
                            for (int i = 0; i < 5; i++){
                                if (compareChar("alloc"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_21);
                                    goto out_peg_23;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			  	  *x = 4;
                                				  value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_23:
                goto out_peg_25;
                success_peg_4:
                ;
                k = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_25;
                }
            
            
            
            {
                    int position_peg_30 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ","));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(","[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_30);
                            goto out_peg_31;
                        }
                    }
                        
                }
                goto success_peg_28;
                out_peg_31:
                goto out_peg_25;
                success_peg_28:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_25;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = k;
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Start = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Start at position " << result_peg_2.getPosition() << " alternative: k:Keyword Spacing COMMA Spacing {{ value = k; }}" << std::endl;
        return result_peg_2;
        out_peg_25:
    
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Start = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Specification(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk3 != 0 && column_peg_1.chunk3->chunk_Specification.calculated()){
        return column_peg_1.chunk3->chunk_Specification;
    }
    
    RuleTrace trace_peg_8(stream, "Specification");
    int myposition = position;
    
    
    Value s;
        Value l;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Specification at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: s:Start* l:Last {{\n			std::vector<bool> specs (4, false);\n			for(Value::iterator it = s.getValues().begin(); it != s.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				int* x = (int*) v.getValue();\n				if(!x)\n				{\n					printf(\"error x is null\\n\");\n					exit(0);       \n				}\n				specs[*x -1] = true;\n				 \n			}\n			int * y = (int *) l.getValue();\n			if(!y) { printf(\"error y is null\\n\"); exit(0); }\n			specs[*y-1] = true;\n			value = &specs; }}" << std::endl;
        {
        
            result_peg_2.reset();
                do{
                    Result result_peg_5(result_peg_2.getPosition());
                    result_peg_5 = rule_Start(stream, result_peg_5.getPosition());
                    if (result_peg_5.error()){
                        goto loop_peg_4;
                    }
                    result_peg_2.addResult(result_peg_5);
                } while (true);
                loop_peg_4:
                ;
                s = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Last(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_7;
                }
                l = result_peg_2.getValues();
            
            
            
            {
                    Value value((void*) 0);
                    std::vector<bool> specs (4, false);
                    			for(Value::iterator it = s.getValues().begin(); it != s.getValues().end(); it ++)
                    			{
                    				const Value & v = *it;
                    				int* x = (int*) v.getValue();
                    				if(!x)
                    				{
                    					printf("error x is null\n");
                    					exit(0);       
                    				}
                    				specs[*x -1] = true;
                    				 
                    			}
                    			int * y = (int *) l.getValue();
                    			if(!y) { printf("error y is null\n"); exit(0); }
                    			specs[*y-1] = true;
                    			value = &specs;
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Specification = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Specification at position " << result_peg_2.getPosition() << " alternative: s:Start* l:Last {{\n			std::vector<bool> specs (4, false);\n			for(Value::iterator it = s.getValues().begin(); it != s.getValues().end(); it ++)\n			{\n				const Value & v = *it;\n				int* x = (int*) v.getValue();\n				if(!x)\n				{\n					printf(\"error x is null\\n\");\n					exit(0);       \n				}\n				specs[*x -1] = true;\n				 \n			}\n			int * y = (int *) l.getValue();\n			if(!y) { printf(\"error y is null\\n\"); exit(0); }\n			specs[*y-1] = true;\n			value = &specs; }}" << std::endl;
        return result_peg_2;
        out_peg_7:
    
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_Specification = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_ProjField(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk3 != 0 && column_peg_1.chunk3->chunk_ProjField.calculated()){
        return column_peg_1.chunk3->chunk_ProjField;
    }
    
    RuleTrace trace_peg_25(stream, "ProjField");
    int myposition = position;
    
    
    Value s;
        Value t;
        Value id;
    Result result_peg_2(myposition);
        std::cout << "Trying rule ProjField at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: OPENS Spacing s:Specification CLOSES Spacing t:Type_form id:Identifier SEMI Spacing {{ int * spec = (int *)s.getValue();\n			bool in = false; bool out = false; bool bind = false; bool alloc = false;\n			std::vector<bool>* specs = (std::vector<bool>*) s.getValue();\n			int i = 0;\n			for(std::vector<bool>::iterator it = specs->begin(); it != specs->end(); it ++)\n			{\n				if(*it)\n				{\n				if(i == 0)\n				     out = true;\n				else if(i == 1)\n				     in = true;\n				else if(i == 2)\n				     bind = true;\n				else\n					alloc = true;\n				}			\n			}\n			value = new ProjectionField(in, out, alloc, bind, (Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "["));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("["[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_6);
                            goto out_peg_7;
                        }
                    }
                        
                }
                goto success_peg_4;
                out_peg_7:
                goto out_peg_8;
                success_peg_4:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
            
            
            
            result_peg_2 = rule_Specification(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
                s = result_peg_2.getValues();
            
            
            
            {
                    int position_peg_14 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "]"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar("]"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_14);
                            goto out_peg_15;
                        }
                    }
                        
                }
                goto success_peg_12;
                out_peg_15:
                goto out_peg_8;
                success_peg_12:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
            
            
            
            result_peg_2 = rule_Type_form(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
                t = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
                id = result_peg_2.getValues();
            
            
            
            {
                    int position_peg_22 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ";"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(";"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_22);
                            goto out_peg_23;
                        }
                    }
                        
                }
                goto success_peg_20;
                out_peg_23:
                goto out_peg_8;
                success_peg_20:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
            
            
            
            {
                    Value value((void*) 0);
                    int * spec = (int *)s.getValue();
                    			bool in = false; bool out = false; bool bind = false; bool alloc = false;
                    			std::vector<bool>* specs = (std::vector<bool>*) s.getValue();
                    			int i = 0;
                    			for(std::vector<bool>::iterator it = specs->begin(); it != specs->end(); it ++)
                    			{
                    				if(*it)
                    				{
                    				if(i == 0)
                    				     out = true;
                    				else if(i == 1)
                    				     in = true;
                    				else if(i == 2)
                    				     bind = true;
                    				else
                    					alloc = true;
                    				}			
                    			}
                    			value = new ProjectionField(in, out, alloc, bind, (Type *) t.getValue(), (char *) id.getValue());
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_ProjField = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule ProjField at position " << result_peg_2.getPosition() << " alternative: OPENS Spacing s:Specification CLOSES Spacing t:Type_form id:Identifier SEMI Spacing {{ int * spec = (int *)s.getValue();\n			bool in = false; bool out = false; bool bind = false; bool alloc = false;\n			std::vector<bool>* specs = (std::vector<bool>*) s.getValue();\n			int i = 0;\n			for(std::vector<bool>::iterator it = specs->begin(); it != specs->end(); it ++)\n			{\n				if(*it)\n				{\n				if(i == 0)\n				     out = true;\n				else if(i == 1)\n				     in = true;\n				else if(i == 2)\n				     bind = true;\n				else\n					alloc = true;\n				}			\n			}\n			value = new ProjectionField(in, out, alloc, bind, (Type *) t.getValue(), (char *) id.getValue()); }}" << std::endl;
        return result_peg_2;
        out_peg_8:
    
        if (column_peg_1.chunk3 == 0){
            column_peg_1.chunk3 = new Chunk3();
        }
        column_peg_1.chunk3->chunk_ProjField = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Last(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk4 != 0 && column_peg_1.chunk4->chunk_Last.calculated()){
        return column_peg_1.chunk4->chunk_Last;
    }
    
    RuleTrace trace_peg_27(stream, "Last");
    int myposition = position;
    
    
    Value k;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Last at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: k:Keyword Spacing {{ value = k; }}" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "out"));
                            for (int i = 0; i < 3; i++){
                                if (compareChar("out"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_6);
                                    goto out_peg_8;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			      *x = 1;
                                			      value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_8:
                {
                    int position_peg_11 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "in"));
                            for (int i = 0; i < 2; i++){
                                if (compareChar("in"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_11);
                                    goto out_peg_13;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			       *x = 2; 
                                			       value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_13:
                {
                    int position_peg_16 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "bind"));
                            for (int i = 0; i < 4; i++){
                                if (compareChar("bind"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_16);
                                    goto out_peg_18;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			  	 *x = 3; 
                                			  	 value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_18:
                {
                    int position_peg_21 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "alloc"));
                            for (int i = 0; i < 5; i++){
                                if (compareChar("alloc"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_21);
                                    goto out_peg_23;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int * x = (int *) malloc(sizeof(int));
                                			  	  *x = 4;
                                				  value = x;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_23:
                goto out_peg_25;
                success_peg_4:
                ;
                k = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_25;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = k;
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Last = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Last at position " << result_peg_2.getPosition() << " alternative: k:Keyword Spacing {{ value = k; }}" << std::endl;
        return result_peg_2;
        out_peg_25:
    
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Last = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Declaration(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk4 != 0 && column_peg_1.chunk4->chunk_Declaration.calculated()){
        return column_peg_1.chunk4->chunk_Declaration;
    }
    
    RuleTrace trace_peg_15(stream, "Declaration");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Declaration at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Type Space+ Identifier Spacing SEMI Spacing" << std::endl;
        {
        
            result_peg_2 = rule_Type(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            {
                    int position_peg_13 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) ";"));
                    for (int i = 0; i < 1; i++){
                        if (compareChar(";"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_13);
                            goto out_peg_14;
                        }
                    }
                        
                }
                goto success_peg_11;
                out_peg_14:
                goto out_peg_4;
                success_peg_11:
                ;
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
        }
        
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Declaration = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Declaration at position " << result_peg_2.getPosition() << " alternative: Type Space+ Identifier Spacing SEMI Spacing" << std::endl;
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Declaration = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Comment(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk4 != 0 && column_peg_1.chunk4->chunk_Comment.calculated()){
        return column_peg_1.chunk4->chunk_Comment;
    }
    
    RuleTrace trace_peg_9(stream, "Comment");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Comment at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Comment_start Comment_rest" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "/*"));
                    for (int i = 0; i < 2; i++){
                        if (compareChar("/*"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_6);
                            goto out_peg_7;
                        }
                    }
                        
                }
                goto success_peg_4;
                out_peg_7:
                goto out_peg_8;
                success_peg_4:
                ;
            
            
            
            result_peg_2 = rule_Comment_rest(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
            
            
        }
        
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Comment = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Comment at position " << result_peg_2.getPosition() << " alternative: Comment_start Comment_rest" << std::endl;
        return result_peg_2;
        out_peg_8:
    
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Comment = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Comment_rest(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk4 != 0 && column_peg_1.chunk4->chunk_Comment_rest.calculated()){
        return column_peg_1.chunk4->chunk_Comment_rest;
    }
    
    RuleTrace trace_peg_13(stream, "Comment_rest");
    int myposition = position;
    
    tail_peg_9:
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Comment_rest at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Comment_end" << std::endl;
        {
            int position_peg_5 = result_peg_2.getPosition();
            
            result_peg_2.setValue(Value((void*) "*/"));
            for (int i = 0; i < 2; i++){
                if (compareChar("*/"[i], stream.get(result_peg_2.getPosition()))){
                    result_peg_2.nextPosition();
                } else {
                    result_peg_2.setPosition(position_peg_5);
                    goto out_peg_6;
                }
            }
                
        }
        goto success_peg_3;
        out_peg_6:
        goto out_peg_7;
        success_peg_3:
        ;
        
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Comment_rest = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Comment_rest at position " << result_peg_2.getPosition() << " alternative: Comment_end" << std::endl;
        return result_peg_2;
        out_peg_7:
        Result result_peg_8(myposition);
        {
        
            char temp_peg_11 = stream.get(result_peg_8.getPosition());
                if (temp_peg_11 != '\0'){
                    result_peg_8.setValue(Value((void*) (long) temp_peg_11));
                    result_peg_8.nextPosition();
                } else {
                    goto out_peg_12;
                }
            
            
            
            
            
            
        }
        myposition = result_peg_8.getPosition();
        goto tail_peg_9;
        out_peg_12:
    
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Comment_rest = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Line_comm_rest(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk4 != 0 && column_peg_1.chunk4->chunk_Line_comm_rest.calculated()){
        return column_peg_1.chunk4->chunk_Line_comm_rest;
    }
    
    RuleTrace trace_peg_13(stream, "Line_comm_rest");
    int myposition = position;
    
    tail_peg_9:
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Line_comm_rest at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Line_comm_end" << std::endl;
        {
            int position_peg_5 = result_peg_2.getPosition();
            
            result_peg_2.setValue(Value((void*) "\n"));
            for (int i = 0; i < 1; i++){
                if (compareChar("\n"[i], stream.get(result_peg_2.getPosition()))){
                    result_peg_2.nextPosition();
                } else {
                    result_peg_2.setPosition(position_peg_5);
                    goto out_peg_6;
                }
            }
                
        }
        goto success_peg_3;
        out_peg_6:
        goto out_peg_7;
        success_peg_3:
        ;
        
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Line_comm_rest = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Line_comm_rest at position " << result_peg_2.getPosition() << " alternative: Line_comm_end" << std::endl;
        return result_peg_2;
        out_peg_7:
        Result result_peg_8(myposition);
        {
        
            char temp_peg_11 = stream.get(result_peg_8.getPosition());
                if (temp_peg_11 != '\0'){
                    result_peg_8.setValue(Value((void*) (long) temp_peg_11));
                    result_peg_8.nextPosition();
                } else {
                    goto out_peg_12;
                }
            
            
            
            
            
            
        }
        myposition = result_peg_8.getPosition();
        goto tail_peg_9;
        out_peg_12:
    
        if (column_peg_1.chunk4 == 0){
            column_peg_1.chunk4 = new Chunk4();
        }
        column_peg_1.chunk4->chunk_Line_comm_rest = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Line_comment(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk5 != 0 && column_peg_1.chunk5->chunk_Line_comment.calculated()){
        return column_peg_1.chunk5->chunk_Line_comment;
    }
    
    RuleTrace trace_peg_9(stream, "Line_comment");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Line_comment at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Line_comm_start Line_comm_rest" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    
                    result_peg_2.setValue(Value((void*) "//"));
                    for (int i = 0; i < 2; i++){
                        if (compareChar("//"[i], stream.get(result_peg_2.getPosition()))){
                            result_peg_2.nextPosition();
                        } else {
                            result_peg_2.setPosition(position_peg_6);
                            goto out_peg_7;
                        }
                    }
                        
                }
                goto success_peg_4;
                out_peg_7:
                goto out_peg_8;
                success_peg_4:
                ;
            
            
            
            result_peg_2 = rule_Line_comm_rest(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_8;
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Line_comment = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Line_comment at position " << result_peg_2.getPosition() << " alternative: Line_comm_start Line_comm_rest" << std::endl;
        return result_peg_2;
        out_peg_8:
    
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Line_comment = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Simple_Type(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk5 != 0 && column_peg_1.chunk5->chunk_Simple_Type.calculated()){
        return column_peg_1.chunk5->chunk_Simple_Type;
    }
    
    RuleTrace trace_peg_24(stream, "Simple_Type");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"int\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kInt; value = ret; }}" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "int"));
                for (int i = 0; i < 3; i++){
                    if (compareChar("int"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kInt; value = ret;
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_2.getPosition() << " alternative: \"int\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kInt; value = ret; }}" << std::endl;
        return result_peg_2;
        out_peg_4:
        Result result_peg_5(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_5.getPosition()) << "' alternative: \"char\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kChar; value = ret; }}" << std::endl;
        {
        
            result_peg_5.setValue(Value((void*) "char"));
                for (int i = 0; i < 4; i++){
                    if (compareChar("char"[i], stream.get(result_peg_5.getPosition()))){
                        result_peg_5.nextPosition();
                    } else {
                        goto out_peg_7;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kChar; value = ret;
                    result_peg_5.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_5;
        stream.update(result_peg_5.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_5.getPosition() << " alternative: \"char\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kChar; value = ret; }}" << std::endl;
        return result_peg_5;
        out_peg_7:
        Result result_peg_8(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_8.getPosition()) << "' alternative: \"capability\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kCapability; value = ret; }}" << std::endl;
        {
        
            result_peg_8.setValue(Value((void*) "capability"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("capability"[i], stream.get(result_peg_8.getPosition()))){
                        result_peg_8.nextPosition();
                    } else {
                        goto out_peg_10;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kCapability; value = ret;
                    result_peg_8.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_8;
        stream.update(result_peg_8.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_8.getPosition() << " alternative: \"capability\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kCapability; value = ret; }}" << std::endl;
        return result_peg_8;
        out_peg_10:
        Result result_peg_11(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_11.getPosition()) << "' alternative: \"short\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kShort; value = ret; }}" << std::endl;
        {
        
            result_peg_11.setValue(Value((void*) "short"));
                for (int i = 0; i < 5; i++){
                    if (compareChar("short"[i], stream.get(result_peg_11.getPosition()))){
                        result_peg_11.nextPosition();
                    } else {
                        goto out_peg_13;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kShort; value = ret;
                    result_peg_11.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_11;
        stream.update(result_peg_11.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_11.getPosition() << " alternative: \"short\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kShort; value = ret; }}" << std::endl;
        return result_peg_11;
        out_peg_13:
        Result result_peg_14(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_14.getPosition()) << "' alternative: \"long\" Space+ \"long\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kLongLong; value = ret; }}" << std::endl;
        {
        
            result_peg_14.setValue(Value((void*) "long"));
                for (int i = 0; i < 4; i++){
                    if (compareChar("long"[i], stream.get(result_peg_14.getPosition()))){
                        result_peg_14.nextPosition();
                    } else {
                        goto out_peg_16;
                    }
                }
            
            
            
            result_peg_14.reset();
                do{
                    Result result_peg_19(result_peg_14.getPosition());
                    result_peg_19 = rule_Space(stream, result_peg_19.getPosition());
                    if (result_peg_19.error()){
                        goto loop_peg_18;
                    }
                    result_peg_14.addResult(result_peg_19);
                } while (true);
                loop_peg_18:
                if (result_peg_14.matches() == 0){
                    goto out_peg_16;
                }
            
            
            
            result_peg_14.setValue(Value((void*) "long"));
                for (int i = 0; i < 4; i++){
                    if (compareChar("long"[i], stream.get(result_peg_14.getPosition()))){
                        result_peg_14.nextPosition();
                    } else {
                        goto out_peg_16;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kLongLong; value = ret;
                    result_peg_14.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_14;
        stream.update(result_peg_14.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_14.getPosition() << " alternative: \"long\" Space+ \"long\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kLongLong; value = ret; }}" << std::endl;
        return result_peg_14;
        out_peg_16:
        Result result_peg_21(myposition);
        std::cout << "Trying rule Simple_Type at " << myposition << " '" << stream.get(result_peg_21.getPosition()) << "' alternative: \"long\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kLong; value = ret; }}" << std::endl;
        {
        
            result_peg_21.setValue(Value((void*) "long"));
                for (int i = 0; i < 4; i++){
                    if (compareChar("long"[i], stream.get(result_peg_21.getPosition()))){
                        result_peg_21.nextPosition();
                    } else {
                        goto out_peg_23;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    int* ret = (int*) malloc(sizeof(int)); *ret = kLong; value = ret;
                    result_peg_21.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = result_peg_21;
        stream.update(result_peg_21.getPosition());
        
        std::cout << "Succeeded rule Simple_Type at position " << result_peg_21.getPosition() << " alternative: \"long\" {{ int* ret = (int*) malloc(sizeof(int)); *ret = kLong; value = ret; }}" << std::endl;
        return result_peg_21;
        out_peg_23:
    
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Simple_Type = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Type_w_modifier(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk5 != 0 && column_peg_1.chunk5->chunk_Type_w_modifier.calculated()){
        return column_peg_1.chunk5->chunk_Type_w_modifier;
    }
    
    RuleTrace trace_peg_15(stream, "Type_w_modifier");
    int myposition = position;
    
    
    Value m;
        Value s;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Type_w_modifier at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: m:Modifier Space+ s:Simple_Type {{ \n			int* val = (int*) m.getValue();\n			int* sval = (int*) s.getValue();\n			value = new PrimitiveType((TypeModifier) *val,\n			(PrimType) *sval); }}" << std::endl;
        {
        
            {
                    int position_peg_6 = result_peg_2.getPosition();
                    {
                    
                        result_peg_2.setValue(Value((void*) "unsigned"));
                            for (int i = 0; i < 8; i++){
                                if (compareChar("unsigned"[i], stream.get(result_peg_2.getPosition()))){
                                    result_peg_2.nextPosition();
                                } else {
                                    result_peg_2.setPosition(position_peg_6);
                                    goto out_peg_8;
                                }
                            }
                        
                        
                        
                        {
                                Value value((void*) 0);
                                int* ret = (int*) malloc(sizeof(int));
                                			*ret = kUnsigned;
                                			 value = ret;
                                result_peg_2.setValue(value);
                            }
                        
                        
                    }
                }
                goto success_peg_4;
                out_peg_8:
                goto out_peg_10;
                success_peg_4:
                ;
                m = result_peg_2.getValues();
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_13(result_peg_2.getPosition());
                    result_peg_13 = rule_Space(stream, result_peg_13.getPosition());
                    if (result_peg_13.error()){
                        goto loop_peg_12;
                    }
                    result_peg_2.addResult(result_peg_13);
                } while (true);
                loop_peg_12:
                if (result_peg_2.matches() == 0){
                    goto out_peg_10;
                }
            
            
            
            result_peg_2 = rule_Simple_Type(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_10;
                }
                s = result_peg_2.getValues();
            
            
            
            {
                    Value value((void*) 0);
                    int* val = (int*) m.getValue();
                    			int* sval = (int*) s.getValue();
                    			value = new PrimitiveType((TypeModifier) *val,
                    			(PrimType) *sval);
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_w_modifier = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Type_w_modifier at position " << result_peg_2.getPosition() << " alternative: m:Modifier Space+ s:Simple_Type {{ \n			int* val = (int*) m.getValue();\n			int* sval = (int*) s.getValue();\n			value = new PrimitiveType((TypeModifier) *val,\n			(PrimType) *sval); }}" << std::endl;
        return result_peg_2;
        out_peg_10:
    
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_w_modifier = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Type(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk5 != 0 && column_peg_1.chunk5->chunk_Type.calculated()){
        return column_peg_1.chunk5->chunk_Type;
    }
    
    RuleTrace trace_peg_7(stream, "Type");
    int myposition = position;
    
    
    Value s;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Type at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Type_w_modifier" << std::endl;
        result_peg_2 = rule_Type_w_modifier(stream, result_peg_2.getPosition());
        if (result_peg_2.error()){
            goto out_peg_3;
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Type at position " << result_peg_2.getPosition() << " alternative: Type_w_modifier" << std::endl;
        return result_peg_2;
        out_peg_3:
        Result result_peg_4(myposition);
        std::cout << "Trying rule Type at " << myposition << " '" << stream.get(result_peg_4.getPosition()) << "' alternative: s:Simple_Type {{ \n			int* sval = (int*) s.getValue();\n			value = new PrimitiveType(kNone, (PrimType) *sval); }}" << std::endl;
        {
        
            result_peg_4 = rule_Simple_Type(stream, result_peg_4.getPosition());
                if (result_peg_4.error()){
                    goto out_peg_6;
                }
                s = result_peg_4.getValues();
            
            
            
            {
                    Value value((void*) 0);
                    int* sval = (int*) s.getValue();
                    			value = new PrimitiveType(kNone, (PrimType) *sval);
                    result_peg_4.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type = result_peg_4;
        stream.update(result_peg_4.getPosition());
        
        std::cout << "Succeeded rule Type at position " << result_peg_4.getPosition() << " alternative: s:Simple_Type {{ \n			int* sval = (int*) s.getValue();\n			value = new PrimitiveType(kNone, (PrimType) *sval); }}" << std::endl;
        return result_peg_4;
        out_peg_6:
    
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Type_form(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk5 != 0 && column_peg_1.chunk5->chunk_Type_form.calculated()){
        return column_peg_1.chunk5->chunk_Type_form;
    }
    
    RuleTrace trace_peg_50(stream, "Type_form");
    int myposition = position;
    
    
    Value m;
        Value name;
        Value t;
    Result result_peg_2(myposition);
        std::cout << "Trying rule Type_form at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \"projection\" Space+ m:Identifier \"::\" name:Identifier Spacing \"*\" Spacing" << std::endl;
        {
        
            result_peg_2.setValue(Value((void*) "projection"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("projection"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2.reset();
                do{
                    Result result_peg_7(result_peg_2.getPosition());
                    result_peg_7 = rule_Space(stream, result_peg_7.getPosition());
                    if (result_peg_7.error()){
                        goto loop_peg_6;
                    }
                    result_peg_2.addResult(result_peg_7);
                } while (true);
                loop_peg_6:
                if (result_peg_2.matches() == 0){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                m = result_peg_2.getValues();
            
            
            
            result_peg_2.setValue(Value((void*) "::"));
                for (int i = 0; i < 2; i++){
                    if (compareChar("::"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Identifier(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
                name = result_peg_2.getValues();
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
            
            result_peg_2.setValue(Value((void*) "*"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("*"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_4;
                    }
                }
            
            
            
            result_peg_2 = rule_Spacing(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Type_form at position " << result_peg_2.getPosition() << " alternative: \"projection\" Space+ m:Identifier \"::\" name:Identifier Spacing \"*\" Spacing" << std::endl;
        return result_peg_2;
        out_peg_4:
        Result result_peg_13(myposition);
        std::cout << "Trying rule Type_form at " << myposition << " '" << stream.get(result_peg_13.getPosition()) << "' alternative: \"projection\" Space+ m:Identifier \"::\" name:Identifier Space+" << std::endl;
        {
        
            result_peg_13.setValue(Value((void*) "projection"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("projection"[i], stream.get(result_peg_13.getPosition()))){
                        result_peg_13.nextPosition();
                    } else {
                        goto out_peg_15;
                    }
                }
            
            
            
            result_peg_13.reset();
                do{
                    Result result_peg_18(result_peg_13.getPosition());
                    result_peg_18 = rule_Space(stream, result_peg_18.getPosition());
                    if (result_peg_18.error()){
                        goto loop_peg_17;
                    }
                    result_peg_13.addResult(result_peg_18);
                } while (true);
                loop_peg_17:
                if (result_peg_13.matches() == 0){
                    goto out_peg_15;
                }
            
            
            
            result_peg_13 = rule_Identifier(stream, result_peg_13.getPosition());
                if (result_peg_13.error()){
                    goto out_peg_15;
                }
                m = result_peg_13.getValues();
            
            
            
            result_peg_13.setValue(Value((void*) "::"));
                for (int i = 0; i < 2; i++){
                    if (compareChar("::"[i], stream.get(result_peg_13.getPosition()))){
                        result_peg_13.nextPosition();
                    } else {
                        goto out_peg_15;
                    }
                }
            
            
            
            result_peg_13 = rule_Identifier(stream, result_peg_13.getPosition());
                if (result_peg_13.error()){
                    goto out_peg_15;
                }
                name = result_peg_13.getValues();
            
            
            
            result_peg_13.reset();
                do{
                    Result result_peg_23(result_peg_13.getPosition());
                    result_peg_23 = rule_Space(stream, result_peg_23.getPosition());
                    if (result_peg_23.error()){
                        goto loop_peg_22;
                    }
                    result_peg_13.addResult(result_peg_23);
                } while (true);
                loop_peg_22:
                if (result_peg_13.matches() == 0){
                    goto out_peg_15;
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = result_peg_13;
        stream.update(result_peg_13.getPosition());
        
        std::cout << "Succeeded rule Type_form at position " << result_peg_13.getPosition() << " alternative: \"projection\" Space+ m:Identifier \"::\" name:Identifier Space+" << std::endl;
        return result_peg_13;
        out_peg_15:
        Result result_peg_24(myposition);
        std::cout << "Trying rule Type_form at " << myposition << " '" << stream.get(result_peg_24.getPosition()) << "' alternative: \"projection\" Space+ name:Identifier Spacing \"*\" Spacing {{\n			value = new ProjectionType((char *)name.getValue(), 1); }}" << std::endl;
        {
        
            result_peg_24.setValue(Value((void*) "projection"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("projection"[i], stream.get(result_peg_24.getPosition()))){
                        result_peg_24.nextPosition();
                    } else {
                        goto out_peg_26;
                    }
                }
            
            
            
            result_peg_24.reset();
                do{
                    Result result_peg_29(result_peg_24.getPosition());
                    result_peg_29 = rule_Space(stream, result_peg_29.getPosition());
                    if (result_peg_29.error()){
                        goto loop_peg_28;
                    }
                    result_peg_24.addResult(result_peg_29);
                } while (true);
                loop_peg_28:
                if (result_peg_24.matches() == 0){
                    goto out_peg_26;
                }
            
            
            
            result_peg_24 = rule_Identifier(stream, result_peg_24.getPosition());
                if (result_peg_24.error()){
                    goto out_peg_26;
                }
                name = result_peg_24.getValues();
            
            
            
            result_peg_24 = rule_Spacing(stream, result_peg_24.getPosition());
                if (result_peg_24.error()){
                    goto out_peg_26;
                }
            
            
            
            result_peg_24.setValue(Value((void*) "*"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("*"[i], stream.get(result_peg_24.getPosition()))){
                        result_peg_24.nextPosition();
                    } else {
                        goto out_peg_26;
                    }
                }
            
            
            
            result_peg_24 = rule_Spacing(stream, result_peg_24.getPosition());
                if (result_peg_24.error()){
                    goto out_peg_26;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new ProjectionType((char *)name.getValue(), 1);
                    result_peg_24.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = result_peg_24;
        stream.update(result_peg_24.getPosition());
        
        std::cout << "Succeeded rule Type_form at position " << result_peg_24.getPosition() << " alternative: \"projection\" Space+ name:Identifier Spacing \"*\" Spacing {{\n			value = new ProjectionType((char *)name.getValue(), 1); }}" << std::endl;
        return result_peg_24;
        out_peg_26:
        Result result_peg_34(myposition);
        std::cout << "Trying rule Type_form at " << myposition << " '" << stream.get(result_peg_34.getPosition()) << "' alternative: \"projection\" Space+ name:Identifier Space+ {{\n			value = new ProjectionType((char *)name.getValue(), 0); }}" << std::endl;
        {
        
            result_peg_34.setValue(Value((void*) "projection"));
                for (int i = 0; i < 10; i++){
                    if (compareChar("projection"[i], stream.get(result_peg_34.getPosition()))){
                        result_peg_34.nextPosition();
                    } else {
                        goto out_peg_36;
                    }
                }
            
            
            
            result_peg_34.reset();
                do{
                    Result result_peg_39(result_peg_34.getPosition());
                    result_peg_39 = rule_Space(stream, result_peg_39.getPosition());
                    if (result_peg_39.error()){
                        goto loop_peg_38;
                    }
                    result_peg_34.addResult(result_peg_39);
                } while (true);
                loop_peg_38:
                if (result_peg_34.matches() == 0){
                    goto out_peg_36;
                }
            
            
            
            result_peg_34 = rule_Identifier(stream, result_peg_34.getPosition());
                if (result_peg_34.error()){
                    goto out_peg_36;
                }
                name = result_peg_34.getValues();
            
            
            
            result_peg_34.reset();
                do{
                    Result result_peg_43(result_peg_34.getPosition());
                    result_peg_43 = rule_Space(stream, result_peg_43.getPosition());
                    if (result_peg_43.error()){
                        goto loop_peg_42;
                    }
                    result_peg_34.addResult(result_peg_43);
                } while (true);
                loop_peg_42:
                if (result_peg_34.matches() == 0){
                    goto out_peg_36;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = new ProjectionType((char *)name.getValue(), 0);
                    result_peg_34.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = result_peg_34;
        stream.update(result_peg_34.getPosition());
        
        std::cout << "Succeeded rule Type_form at position " << result_peg_34.getPosition() << " alternative: \"projection\" Space+ name:Identifier Space+ {{\n			value = new ProjectionType((char *)name.getValue(), 0); }}" << std::endl;
        return result_peg_34;
        out_peg_36:
        Result result_peg_44(myposition);
        std::cout << "Trying rule Type_form at " << myposition << " '" << stream.get(result_peg_44.getPosition()) << "' alternative: t:Type Space+ {{\n			value = (PrimitiveType *) t.getValue(); }}" << std::endl;
        {
        
            result_peg_44 = rule_Type(stream, result_peg_44.getPosition());
                if (result_peg_44.error()){
                    goto out_peg_46;
                }
                t = result_peg_44.getValues();
            
            
            
            result_peg_44.reset();
                do{
                    Result result_peg_49(result_peg_44.getPosition());
                    result_peg_49 = rule_Space(stream, result_peg_49.getPosition());
                    if (result_peg_49.error()){
                        goto loop_peg_48;
                    }
                    result_peg_44.addResult(result_peg_49);
                } while (true);
                loop_peg_48:
                if (result_peg_44.matches() == 0){
                    goto out_peg_46;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = (PrimitiveType *) t.getValue();
                    result_peg_44.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = result_peg_44;
        stream.update(result_peg_44.getPosition());
        
        std::cout << "Succeeded rule Type_form at position " << result_peg_44.getPosition() << " alternative: t:Type Space+ {{\n			value = (PrimitiveType *) t.getValue(); }}" << std::endl;
        return result_peg_44;
        out_peg_46:
    
        if (column_peg_1.chunk5 == 0){
            column_peg_1.chunk5 = new Chunk5();
        }
        column_peg_1.chunk5->chunk_Type_form = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Spacing(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk6 != 0 && column_peg_1.chunk6->chunk_Spacing.calculated()){
        return column_peg_1.chunk6->chunk_Spacing;
    }
    
    RuleTrace trace_peg_5(stream, "Spacing");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Spacing at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: Space*" << std::endl;
        result_peg_2.reset();
        do{
            Result result_peg_4(result_peg_2.getPosition());
            result_peg_4 = rule_Space(stream, result_peg_4.getPosition());
            if (result_peg_4.error()){
                goto loop_peg_3;
            }
            result_peg_2.addResult(result_peg_4);
        } while (true);
        loop_peg_3:
        ;
        
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Spacing = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Spacing at position " << result_peg_2.getPosition() << " alternative: Space*" << std::endl;
        return result_peg_2;
    
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Spacing = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_Space(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk6 != 0 && column_peg_1.chunk6->chunk_Space.calculated()){
        return column_peg_1.chunk6->chunk_Space;
    }
    
    RuleTrace trace_peg_15(stream, "Space");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        std::cout << "Trying rule Space at " << myposition << " '" << stream.get(result_peg_2.getPosition()) << "' alternative: \" \"" << std::endl;
        result_peg_2.setValue(Value((void*) " "));
        for (int i = 0; i < 1; i++){
            if (compareChar(" "[i], stream.get(result_peg_2.getPosition()))){
                result_peg_2.nextPosition();
            } else {
                goto out_peg_3;
            }
        }
        
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Space = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        std::cout << "Succeeded rule Space at position " << result_peg_2.getPosition() << " alternative: \" \"" << std::endl;
        return result_peg_2;
        out_peg_3:
        Result result_peg_4(myposition);
        std::cout << "Trying rule Space at " << myposition << " '" << stream.get(result_peg_4.getPosition()) << "' alternative: \"\\t\"" << std::endl;
        result_peg_4.setValue(Value((void*) "\t"));
        for (int i = 0; i < 1; i++){
            if (compareChar("\t"[i], stream.get(result_peg_4.getPosition()))){
                result_peg_4.nextPosition();
            } else {
                goto out_peg_5;
            }
        }
        
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Space = result_peg_4;
        stream.update(result_peg_4.getPosition());
        
        std::cout << "Succeeded rule Space at position " << result_peg_4.getPosition() << " alternative: \"\\t\"" << std::endl;
        return result_peg_4;
        out_peg_5:
        Result result_peg_6(myposition);
        std::cout << "Trying rule Space at " << myposition << " '" << stream.get(result_peg_6.getPosition()) << "' alternative: EndOfLine" << std::endl;
        {
            int position_peg_9 = result_peg_6.getPosition();
            
            result_peg_6.setValue(Value((void*) "\n"));
            for (int i = 0; i < 1; i++){
                if (compareChar("\n"[i], stream.get(result_peg_6.getPosition()))){
                    result_peg_6.nextPosition();
                } else {
                    result_peg_6.setPosition(position_peg_9);
                    goto out_peg_10;
                }
            }
                
        }
        goto success_peg_7;
        out_peg_10:
        {
            int position_peg_12 = result_peg_6.getPosition();
            
            result_peg_6.setValue(Value((void*) "\r"));
            for (int i = 0; i < 1; i++){
                if (compareChar("\r"[i], stream.get(result_peg_6.getPosition()))){
                    result_peg_6.nextPosition();
                } else {
                    result_peg_6.setPosition(position_peg_12);
                    goto out_peg_13;
                }
            }
                
        }
        goto success_peg_7;
        out_peg_13:
        goto out_peg_14;
        success_peg_7:
        ;
        
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Space = result_peg_6;
        stream.update(result_peg_6.getPosition());
        
        std::cout << "Succeeded rule Space at position " << result_peg_6.getPosition() << " alternative: EndOfLine" << std::endl;
        return result_peg_6;
        out_peg_14:
    
        if (column_peg_1.chunk6 == 0){
            column_peg_1.chunk6 = new Chunk6();
        }
        column_peg_1.chunk6->chunk_Space = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

static const void * doParse(Stream & stream, bool stats, const std::string & context){
errorResult.setError();
Result done = rule_File(stream, 0);
if (done.error()){
    stream.reportError(context);
}
if (stats){
    stream.printStats();
}
return done.getValues().getValue();
}

const void * parse(const std::string & filename, bool stats = false){
Stream stream(filename);
return doParse(stream, stats, filename);
}

const void * parse(const char * in, bool stats = false){
Stream stream(in);
return doParse(stream, stats, "memory");
}

const void * parse(const char * in, int length, bool stats = false){
Stream stream(in, length);
return doParse(stream, stats, "memory");
}



} /* Parser */

    
