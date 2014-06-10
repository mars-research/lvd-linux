
import peg





def special_escape(s):
    return s.replace("\\n", "\\\\n").replace("\\t", "\\\\t").replace("\"", '\\\"').replace("\\r", "\\\\r")

class PegError(Exception):
    def __init__(self):
        Exception.__init__(self)

class NotError(Exception):
    def __init__(self):
        Exception.__init__(self)

class Result:
    def __init__(self, position):
        self.position = position
        self.values = []

    def getPosition(self):
        return self.position

    def nextPosition(self, amount = 1):
        self.position += amount

    def setValue(self, value):
        self.values = value

    def getLastValue(self):
        if type(self.values) is list:
            if len(self.values) > 0:
                return self.values[-1]
            else:
                return None
        return self.values
    
    def matches(self):
        return len(self.values)

    def getValues(self):
        return self.values

    def addResult(self, him):
        self.values.append(him.values)
        self.position = him.position
    
    #def extendResult(self, him):
    #    self.values.extend(him.values)
    #    self.position = him.position

class Stream:
    def __init__(self, filename = None, input = None):
        def read():
            file = open(filename, 'r')
            out = file.read()
            file.close()
            return out
        self.position = 0
        self.limit = 100
        self.furthest = 0
        self.memo = {}
        if filename != None:
            self.all = read()
        elif input != None:
            self.all = input
        else:
            raise PegError("Pass a filename or input")
        # print "Read " + str(len(self.all))

    def get(self, position, number = 1):
        if position + number > self.limit:
            # print (position + number)
            self.limit += 5000
        if position + number > len(self.all):
            return chr(0)
        # print "stream: %s" % self.all[position:position+number]
        return self.all[position:position+number]

    def get2(self, position):
        if position != self.position:
            self.file.seek(position)
        self.position = position + 1
        if position > self.limit:
            print position
            self.limit += 5000
        return self.file.read(1)

    def reportError(self):
        line = 1
        column = 1
        for i in xrange(0, self.furthest):
            if self.all[i] == '\n':
                line += 1
                column = 1
            else:
                column += 1
        context = 10
        left = self.furthest - context
        right = self.furthest + context
        if left < 0:
            left = 0
        if right > len(self.all):
            right = len(self.all)
        print "Read up till line %d, column %d" % (line, column)
        print "'%s'" % special_escape(self.all[left:right])
        print "%s^" % (' ' * (self.furthest - left))

    def update(self, rule, position, result):
        if result != None and result.getPosition() > self.furthest:
            self.furthest = result.getPosition()

        for_rule = None
        try:
            for_rule = self.memo[rule]
        except KeyError:
            self.memo[rule] = {}
            for_rule = self.memo[rule]
        
        for_position = None
        try:
            for_position = for_rule[position]
        except KeyError:
            for_rule[position] = None

        for_rule[position] = result

    def hasResult(self, rule, position):
        try:
            x = self.memo[rule][position]
            return True
        except KeyError:
            return False

    def result(self, rule, position):
        return self.memo[rule][position]



RULE_start = 0
RULE_module = 1
RULE_include = 2
RULE_more_code = 3
RULE_options = 4
RULE_option = 5
RULE_error_option = 6
RULE_word = 7
RULE_rules = 8
RULE_rule = 9
RULE_pattern_line = 10
RULE_pattern = 11
RULE_raw_code = 12
RULE_code = 13
RULE_item = 14
RULE_failure = 15
RULE_line = 16
RULE_predicate = 17
RULE_utf8 = 18
RULE_ascii = 19
RULE_call_rule = 20
RULE_eof = 21
RULE_void = 22
RULE_range = 23
RULE_sub_pattern = 24
RULE_bind = 25
RULE_string = 26
RULE_modifier = 27
RULE_x_word = 28
RULE_rule_parameters = 29
RULE_value_parameters = 30
RULE_parameters_rules = 31
RULE_parameters_values = 32
RULE_word_or_dollar = 33
RULE_word_or_at = 34
RULE_word_at = 35
RULE_dollar = 36
RULE_number = 37
RULE_digit = 38
RULE_hex_number = 39
RULE_hex_digit = 40
RULE_start_symbol = 41
RULE_spaces = 42
RULE_space = 43
RULE_any_char = 44
RULE_any = 45
RULE_whitespace = 46
RULE_comment = 47
RULE_newlines_one = 48
RULE_newlines = 49




def rule_start(stream, position):
    if stream.hasResult(RULE_start, position):
        return stream.result(RULE_start, position)
    try:
        result_peg_1 = Result(position)
        result_peg_2 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_2 = rule_newlines(stream, result_peg_2.getPosition())
        if result_peg_2 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_2);
        
        result_peg_3 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_3 = rule_whitespace(stream, result_peg_3.getPosition())
        if result_peg_3 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_3);
        
        result_peg_4 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'start_symbol'
        result_peg_4 = rule_start_symbol(stream, result_peg_4.getPosition())
        if result_peg_4 == None:
            raise PegError
        start_symbol = result_peg_4.getValues()
        
        result_peg_1.addResult(result_peg_4);
        
        result_peg_5 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_5 = rule_newlines(stream, result_peg_5.getPosition())
        if result_peg_5 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_5);
        
        result_peg_6 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_6 = rule_whitespace(stream, result_peg_6.getPosition())
        if result_peg_6 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_6);
        
        result_peg_7 = Result(result_peg_1.getPosition())
        
        save_peg_8 = result_peg_7.getPosition()
        
        # print "Trying rule " + 'options'
        result_peg_7 = rule_options(stream, result_peg_7.getPosition())
        if result_peg_7 == None:
            
            result_peg_7 = Result(save_peg_8)
            result_peg_7.setValue(None)
        options = result_peg_7.getValues()
        
        result_peg_1.addResult(result_peg_7);
        
        result_peg_9 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_9 = rule_newlines(stream, result_peg_9.getPosition())
        if result_peg_9 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_9);
        
        result_peg_10 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_10 = rule_whitespace(stream, result_peg_10.getPosition())
        if result_peg_10 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_10);
        
        result_peg_11 = Result(result_peg_1.getPosition())
        
        save_peg_12 = result_peg_11.getPosition()
        
        # print "Trying rule " + 'module'
        result_peg_11 = rule_module(stream, result_peg_11.getPosition())
        if result_peg_11 == None:
            
            result_peg_11 = Result(save_peg_12)
            result_peg_11.setValue(None)
        module = result_peg_11.getValues()
        
        result_peg_1.addResult(result_peg_11);
        
        result_peg_13 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_13 = rule_newlines(stream, result_peg_13.getPosition())
        if result_peg_13 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_13);
        
        result_peg_14 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_14 = rule_whitespace(stream, result_peg_14.getPosition())
        if result_peg_14 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_14);
        
        result_peg_15 = Result(result_peg_1.getPosition())
        
        save_peg_16 = result_peg_15.getPosition()
        
        # print "Trying rule " + 'include'
        result_peg_15 = rule_include(stream, result_peg_15.getPosition())
        if result_peg_15 == None:
            
            result_peg_15 = Result(save_peg_16)
            result_peg_15.setValue(None)
        include = result_peg_15.getValues()
        
        result_peg_1.addResult(result_peg_15);
        
        result_peg_17 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_17 = rule_newlines(stream, result_peg_17.getPosition())
        if result_peg_17 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_17);
        
        result_peg_18 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_18 = rule_whitespace(stream, result_peg_18.getPosition())
        if result_peg_18 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_18);
        
        result_peg_19 = Result(result_peg_1.getPosition())
        
        save_peg_20 = result_peg_19.getPosition()
        
        # print "Trying rule " + 'more_code'
        result_peg_19 = rule_more_code(stream, result_peg_19.getPosition())
        if result_peg_19 == None:
            
            result_peg_19 = Result(save_peg_20)
            result_peg_19.setValue(None)
        code = result_peg_19.getValues()
        
        result_peg_1.addResult(result_peg_19);
        
        result_peg_21 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_21 = rule_newlines(stream, result_peg_21.getPosition())
        if result_peg_21 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_21);
        
        result_peg_22 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_22 = rule_whitespace(stream, result_peg_22.getPosition())
        if result_peg_22 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_22);
        
        result_peg_23 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'rules'
        result_peg_23 = rule_rules(stream, result_peg_23.getPosition())
        if result_peg_23 == None:
            raise PegError
        rules = result_peg_23.getValues()
        
        result_peg_1.addResult(result_peg_23);
        
        result_peg_24 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'newlines'
        result_peg_24 = rule_newlines(stream, result_peg_24.getPosition())
        if result_peg_24 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_24);
        
        result_peg_25 = Result(result_peg_1.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_25 = rule_whitespace(stream, result_peg_25.getPosition())
        if result_peg_25 == None:
            raise PegError
        
        result_peg_1.addResult(result_peg_25);
        
        result_peg_26 = Result(result_peg_1.getPosition())
        
        if chr(0) == stream.get(result_peg_26.getPosition()):
            result_peg_26.nextPosition()
            result_peg_26.setValue(chr(0))
        else:
            raise PegError
        
        result_peg_1.addResult(result_peg_26);
        
        result_peg_27 = Result(result_peg_1.getPosition())
        
        value = None
        values = result_peg_1.getValues()
        value = peg.Peg(start_symbol, include, code, module, rules, options)
        result_peg_27.setValue(value)
        
        result_peg_1.addResult(result_peg_27);
        
        result_peg_1.setValue(result_peg_1.getLastValue())
        stream.update(RULE_start, position, result_peg_1)
        return result_peg_1
    except PegError:
        pass
    stream.update(RULE_start, position, None)
    return None


def rule_module(stream, position):
    if stream.hasResult(RULE_module, position):
        return stream.result(RULE_module, position)
    try:
        result_peg_28 = Result(position)
        result_peg_29 = Result(result_peg_28.getPosition())
        
        if 'module:' == stream.get(result_peg_29.getPosition(), 7):
            result_peg_29.nextPosition(7)
            result_peg_29.setValue('module:')
        else:
            raise PegError
        
        result_peg_28.addResult(result_peg_29);
        
        result_peg_30 = Result(result_peg_28.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_30 = rule_spaces(stream, result_peg_30.getPosition())
        if result_peg_30 == None:
            raise PegError
        
        result_peg_28.addResult(result_peg_30);
        
        result_peg_31 = Result(result_peg_28.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_31 = rule_word(stream, result_peg_31.getPosition())
        if result_peg_31 == None:
            raise PegError
        name = result_peg_31.getValues()
        
        result_peg_28.addResult(result_peg_31);
        
        result_peg_32 = Result(result_peg_28.getPosition())
        
        try:
            while True:
                result_peg_33 = Result(result_peg_32.getPosition());
                result_peg_34 = Result(result_peg_33.getPosition())
                
                if '.' == stream.get(result_peg_34.getPosition(), 1):
                    result_peg_34.nextPosition(1)
                    result_peg_34.setValue('.')
                else:
                    raise PegError
                
                result_peg_33.addResult(result_peg_34);
                
                result_peg_35 = Result(result_peg_33.getPosition())
                
                # print "Trying rule " + 'word'
                result_peg_35 = rule_word(stream, result_peg_35.getPosition())
                if result_peg_35 == None:
                    raise PegError
                
                result_peg_33.addResult(result_peg_35);
                
                result_peg_36 = Result(result_peg_33.getPosition())
                
                value = None
                values = result_peg_33.getValues()
                value = values[1]
                result_peg_36.setValue(value)
                
                result_peg_33.addResult(result_peg_36);
                
                result_peg_33.setValue(result_peg_33.getLastValue())
                result_peg_32.addResult(result_peg_33);
        except PegError:
            pass
        rest = result_peg_32.getValues()
        
        result_peg_28.addResult(result_peg_32);
        
        result_peg_37 = Result(result_peg_28.getPosition())
        
        value = None
        values = result_peg_28.getValues()
        value = [name] + rest
        result_peg_37.setValue(value)
        
        result_peg_28.addResult(result_peg_37);
        
        result_peg_28.setValue(result_peg_28.getLastValue())
        stream.update(RULE_module, position, result_peg_28)
        return result_peg_28
    except PegError:
        pass
    stream.update(RULE_module, position, None)
    return None


def rule_include(stream, position):
    if stream.hasResult(RULE_include, position):
        return stream.result(RULE_include, position)
    try:
        result_peg_38 = Result(position)
        result_peg_39 = Result(result_peg_38.getPosition())
        
        if 'include:' == stream.get(result_peg_39.getPosition(), 8):
            result_peg_39.nextPosition(8)
            result_peg_39.setValue('include:')
        else:
            raise PegError
        
        result_peg_38.addResult(result_peg_39);
        
        result_peg_40 = Result(result_peg_38.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_40 = rule_spaces(stream, result_peg_40.getPosition())
        if result_peg_40 == None:
            raise PegError
        
        result_peg_38.addResult(result_peg_40);
        
        result_peg_41 = Result(result_peg_38.getPosition())
        
        # print "Trying rule " + 'code'
        result_peg_41 = rule_code(stream, result_peg_41.getPosition())
        if result_peg_41 == None:
            raise PegError
        code = result_peg_41.getValues()
        
        result_peg_38.addResult(result_peg_41);
        
        result_peg_42 = Result(result_peg_38.getPosition())
        
        value = None
        values = result_peg_38.getValues()
        value = code.code
        result_peg_42.setValue(value)
        
        result_peg_38.addResult(result_peg_42);
        
        result_peg_38.setValue(result_peg_38.getLastValue())
        stream.update(RULE_include, position, result_peg_38)
        return result_peg_38
    except PegError:
        pass
    stream.update(RULE_include, position, None)
    return None


def rule_more_code(stream, position):
    if stream.hasResult(RULE_more_code, position):
        return stream.result(RULE_more_code, position)
    try:
        result_peg_43 = Result(position)
        result_peg_44 = Result(result_peg_43.getPosition())
        
        if 'code:' == stream.get(result_peg_44.getPosition(), 5):
            result_peg_44.nextPosition(5)
            result_peg_44.setValue('code:')
        else:
            raise PegError
        
        result_peg_43.addResult(result_peg_44);
        
        result_peg_45 = Result(result_peg_43.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_45 = rule_spaces(stream, result_peg_45.getPosition())
        if result_peg_45 == None:
            raise PegError
        
        result_peg_43.addResult(result_peg_45);
        
        result_peg_46 = Result(result_peg_43.getPosition())
        
        # print "Trying rule " + 'code'
        result_peg_46 = rule_code(stream, result_peg_46.getPosition())
        if result_peg_46 == None:
            raise PegError
        code = result_peg_46.getValues()
        
        result_peg_43.addResult(result_peg_46);
        
        result_peg_47 = Result(result_peg_43.getPosition())
        
        value = None
        values = result_peg_43.getValues()
        value = code.code
        result_peg_47.setValue(value)
        
        result_peg_43.addResult(result_peg_47);
        
        result_peg_43.setValue(result_peg_43.getLastValue())
        stream.update(RULE_more_code, position, result_peg_43)
        return result_peg_43
    except PegError:
        pass
    stream.update(RULE_more_code, position, None)
    return None


def rule_options(stream, position):
    if stream.hasResult(RULE_options, position):
        return stream.result(RULE_options, position)
    try:
        result_peg_48 = Result(position)
        result_peg_49 = Result(result_peg_48.getPosition())
        
        if 'options:' == stream.get(result_peg_49.getPosition(), 8):
            result_peg_49.nextPosition(8)
            result_peg_49.setValue('options:')
        else:
            raise PegError
        
        result_peg_48.addResult(result_peg_49);
        
        result_peg_50 = Result(result_peg_48.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_50 = rule_spaces(stream, result_peg_50.getPosition())
        if result_peg_50 == None:
            raise PegError
        
        result_peg_48.addResult(result_peg_50);
        
        result_peg_51 = Result(result_peg_48.getPosition())
        
        # print "Trying rule " + 'option'
        result_peg_51 = rule_option(stream, result_peg_51.getPosition())
        if result_peg_51 == None:
            raise PegError
        option1 = result_peg_51.getValues()
        
        result_peg_48.addResult(result_peg_51);
        
        result_peg_52 = Result(result_peg_48.getPosition())
        
        try:
            while True:
                result_peg_53 = Result(result_peg_52.getPosition());
                result_peg_54 = Result(result_peg_53.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_54 = rule_spaces(stream, result_peg_54.getPosition())
                if result_peg_54 == None:
                    raise PegError
                
                result_peg_53.addResult(result_peg_54);
                
                result_peg_55 = Result(result_peg_53.getPosition())
                
                if ',' == stream.get(result_peg_55.getPosition(), 1):
                    result_peg_55.nextPosition(1)
                    result_peg_55.setValue(',')
                else:
                    raise PegError
                
                result_peg_53.addResult(result_peg_55);
                
                result_peg_56 = Result(result_peg_53.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_56 = rule_spaces(stream, result_peg_56.getPosition())
                if result_peg_56 == None:
                    raise PegError
                
                result_peg_53.addResult(result_peg_56);
                
                result_peg_57 = Result(result_peg_53.getPosition())
                
                # print "Trying rule " + 'option'
                result_peg_57 = rule_option(stream, result_peg_57.getPosition())
                if result_peg_57 == None:
                    raise PegError
                
                result_peg_53.addResult(result_peg_57);
                
                result_peg_53.setValue(result_peg_53.getLastValue())
                result_peg_52.addResult(result_peg_53);
        except PegError:
            pass
        option_rest = result_peg_52.getValues()
        
        result_peg_48.addResult(result_peg_52);
        
        result_peg_58 = Result(result_peg_48.getPosition())
        
        value = None
        values = result_peg_48.getValues()
        value = []
        for option in ([option1] + option_rest):
            import re
            debug = re.compile("debug(\d+)")
            out = debug.match(option)
            if out != None:
                num = int(out.group(1))
                for x in xrange(1,num+1):
                    value.append('debug%d' % x)
            elif option == 'no-memo':
                value.append(option)
            else:
                value.append(option)
        result_peg_58.setValue(value)
        
        result_peg_48.addResult(result_peg_58);
        
        result_peg_48.setValue(result_peg_48.getLastValue())
        stream.update(RULE_options, position, result_peg_48)
        return result_peg_48
    except PegError:
        pass
    stream.update(RULE_options, position, None)
    return None


def rule_option(stream, position):
    if stream.hasResult(RULE_option, position):
        return stream.result(RULE_option, position)
    try:
        result_peg_59 = Result(position)
        result_peg_60 = Result(result_peg_59.getPosition())
        
        if 'debug' == stream.get(result_peg_60.getPosition(), 5):
            result_peg_60.nextPosition(5)
            result_peg_60.setValue('debug')
        else:
            raise PegError
        
        result_peg_59.addResult(result_peg_60);
        
        result_peg_61 = Result(result_peg_59.getPosition())
        
        # print "Trying rule " + 'number'
        result_peg_61 = rule_number(stream, result_peg_61.getPosition())
        if result_peg_61 == None:
            raise PegError
        number = result_peg_61.getValues()
        
        result_peg_59.addResult(result_peg_61);
        
        result_peg_62 = Result(result_peg_59.getPosition())
        
        value = None
        values = result_peg_59.getValues()
        value = 'debug%s' % number
        result_peg_62.setValue(value)
        
        result_peg_59.addResult(result_peg_62);
        
        result_peg_59.setValue(result_peg_59.getLastValue())
        stream.update(RULE_option, position, result_peg_59)
        return result_peg_59
    except PegError:
        pass
    try:
        result_peg_63 = Result(position)
        if 'no-memo' == stream.get(result_peg_63.getPosition(), 7):
            result_peg_63.nextPosition(7)
            result_peg_63.setValue('no-memo')
        else:
            raise PegError
        stream.update(RULE_option, position, result_peg_63)
        return result_peg_63
    except PegError:
        pass
    try:
        result_peg_64 = Result(position)
        # print "Trying rule " + 'error_option'
        result_peg_64 = rule_error_option(stream, result_peg_64.getPosition())
        if result_peg_64 == None:
            raise PegError
        stream.update(RULE_option, position, result_peg_64)
        return result_peg_64
    except PegError:
        pass
    stream.update(RULE_option, position, None)
    return None


def rule_error_option(stream, position):
    if stream.hasResult(RULE_error_option, position):
        return stream.result(RULE_error_option, position)
    try:
        result_peg_65 = Result(position)
        result_peg_66 = Result(result_peg_65.getPosition())
        
        if 'error-length' == stream.get(result_peg_66.getPosition(), 12):
            result_peg_66.nextPosition(12)
            result_peg_66.setValue('error-length')
        else:
            raise PegError
        
        result_peg_65.addResult(result_peg_66);
        
        result_peg_67 = Result(result_peg_65.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_67 = rule_whitespace(stream, result_peg_67.getPosition())
        if result_peg_67 == None:
            raise PegError
        
        result_peg_65.addResult(result_peg_67);
        
        result_peg_68 = Result(result_peg_65.getPosition())
        
        # print "Trying rule " + 'number'
        result_peg_68 = rule_number(stream, result_peg_68.getPosition())
        if result_peg_68 == None:
            raise PegError
        number = result_peg_68.getValues()
        
        result_peg_65.addResult(result_peg_68);
        
        result_peg_69 = Result(result_peg_65.getPosition())
        
        value = None
        values = result_peg_65.getValues()
        value = 'error-length %s' % number
        result_peg_69.setValue(value)
        
        result_peg_65.addResult(result_peg_69);
        
        result_peg_65.setValue(result_peg_65.getLastValue())
        stream.update(RULE_error_option, position, result_peg_65)
        return result_peg_65
    except PegError:
        pass
    stream.update(RULE_error_option, position, None)
    return None


def rule_word(stream, position):
    if stream.hasResult(RULE_word, position):
        return stream.result(RULE_word, position)
    try:
        result_peg_70 = Result(position)
        result_peg_71 = Result(result_peg_70.getPosition())
        
        try:
            while True:
                result_peg_72 = Result(result_peg_71.getPosition());
                # print "Trying rule " + 'any_char'
                result_peg_72 = rule_any_char(stream, result_peg_72.getPosition())
                if result_peg_72 == None:
                    raise PegError
                result_peg_71.addResult(result_peg_72);
        except PegError:
            if result_peg_71.matches() == 0:
                raise PegError
                
        result_peg_70.addResult(result_peg_71);
        
        result_peg_74 = Result(result_peg_70.getPosition())
        
        value = None
        values = result_peg_70.getValues()
        # print "all start symbol values " + str(values)
        # print "values[0] " + str(values[0])
        value = ''.join(values[0]).replace('-', '__')
        # print "got word " + value
        result_peg_74.setValue(value)
        
        result_peg_70.addResult(result_peg_74);
        
        result_peg_70.setValue(result_peg_70.getLastValue())
        stream.update(RULE_word, position, result_peg_70)
        return result_peg_70
    except PegError:
        pass
    stream.update(RULE_word, position, None)
    return None


def rule_rules(stream, position):
    if stream.hasResult(RULE_rules, position):
        return stream.result(RULE_rules, position)
    try:
        result_peg_75 = Result(position)
        result_peg_76 = Result(result_peg_75.getPosition())
        
        if 'rules:' == stream.get(result_peg_76.getPosition(), 6):
            result_peg_76.nextPosition(6)
            result_peg_76.setValue('rules:')
        else:
            raise PegError
        
        result_peg_75.addResult(result_peg_76);
        
        result_peg_77 = Result(result_peg_75.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_77 = rule_whitespace(stream, result_peg_77.getPosition())
        if result_peg_77 == None:
            raise PegError
        
        result_peg_75.addResult(result_peg_77);
        
        result_peg_78 = Result(result_peg_75.getPosition())
        
        try:
            while True:
                result_peg_79 = Result(result_peg_78.getPosition());
                result_peg_80 = Result(result_peg_79.getPosition())
                
                # print "Trying rule " + 'rule'
                result_peg_80 = rule_rule(stream, result_peg_80.getPosition())
                if result_peg_80 == None:
                    raise PegError
                
                result_peg_79.addResult(result_peg_80);
                
                result_peg_81 = Result(result_peg_79.getPosition())
                
                # print "Trying rule " + 'whitespace'
                result_peg_81 = rule_whitespace(stream, result_peg_81.getPosition())
                if result_peg_81 == None:
                    raise PegError
                
                result_peg_79.addResult(result_peg_81);
                
                result_peg_82 = Result(result_peg_79.getPosition())
                
                value = None
                values = result_peg_79.getValues()
                value = values[0]
                result_peg_82.setValue(value)
                
                result_peg_79.addResult(result_peg_82);
                
                result_peg_79.setValue(result_peg_79.getLastValue())
                result_peg_78.addResult(result_peg_79);
        except PegError:
            pass
        rules = result_peg_78.getValues()
        
        result_peg_75.addResult(result_peg_78);
        
        result_peg_83 = Result(result_peg_75.getPosition())
        
        value = None
        values = result_peg_75.getValues()
        value = rules
        result_peg_83.setValue(value)
        
        result_peg_75.addResult(result_peg_83);
        
        result_peg_75.setValue(result_peg_75.getLastValue())
        stream.update(RULE_rules, position, result_peg_75)
        return result_peg_75
    except PegError:
        pass
    stream.update(RULE_rules, position, None)
    return None


def rule_rule(stream, position):
    if stream.hasResult(RULE_rule, position):
        return stream.result(RULE_rule, position)
    try:
        result_peg_84 = Result(position)
        result_peg_85 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_85 = rule_spaces(stream, result_peg_85.getPosition())
        if result_peg_85 == None:
            raise PegError
        
        result_peg_84.addResult(result_peg_85);
        
        result_peg_86 = Result(result_peg_84.getPosition())
        
        save_peg_87 = result_peg_86.getPosition()
        
        if 'inline' == stream.get(result_peg_86.getPosition(), 6):
            result_peg_86.nextPosition(6)
            result_peg_86.setValue('inline')
        else:
            
            result_peg_86 = Result(save_peg_87)
            result_peg_86.setValue(None)
        inline = result_peg_86.getValues()
        
        result_peg_84.addResult(result_peg_86);
        
        result_peg_88 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_88 = rule_spaces(stream, result_peg_88.getPosition())
        if result_peg_88 == None:
            raise PegError
        
        result_peg_84.addResult(result_peg_88);
        
        result_peg_89 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_89 = rule_word(stream, result_peg_89.getPosition())
        if result_peg_89 == None:
            raise PegError
        name = result_peg_89.getValues()
        
        result_peg_84.addResult(result_peg_89);
        
        result_peg_90 = Result(result_peg_84.getPosition())
        
        save_peg_91 = result_peg_90.getPosition()
        
        # print "Trying rule " + 'rule_parameters'
        result_peg_90 = rule_rule_parameters(stream, result_peg_90.getPosition())
        if result_peg_90 == None:
            
            result_peg_90 = Result(save_peg_91)
            result_peg_90.setValue(None)
        rule_parameters = result_peg_90.getValues()
        
        result_peg_84.addResult(result_peg_90);
        
        result_peg_92 = Result(result_peg_84.getPosition())
        
        save_peg_93 = result_peg_92.getPosition()
        
        # print "Trying rule " + 'value_parameters'
        result_peg_92 = rule_value_parameters(stream, result_peg_92.getPosition())
        if result_peg_92 == None:
            
            result_peg_92 = Result(save_peg_93)
            result_peg_92.setValue(None)
        parameters = result_peg_92.getValues()
        
        result_peg_84.addResult(result_peg_92);
        
        result_peg_94 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_94 = rule_spaces(stream, result_peg_94.getPosition())
        if result_peg_94 == None:
            raise PegError
        
        result_peg_84.addResult(result_peg_94);
        
        result_peg_95 = Result(result_peg_84.getPosition())
        
        if '=' == stream.get(result_peg_95.getPosition(), 1):
            result_peg_95.nextPosition(1)
            result_peg_95.setValue('=')
        else:
            raise PegError
        
        result_peg_84.addResult(result_peg_95);
        
        result_peg_96 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_96 = rule_spaces(stream, result_peg_96.getPosition())
        if result_peg_96 == None:
            raise PegError
        
        result_peg_84.addResult(result_peg_96);
        
        result_peg_97 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'pattern_line'
        result_peg_97 = rule_pattern_line(stream, result_peg_97.getPosition())
        if result_peg_97 == None:
            raise PegError
        pattern1 = result_peg_97.getValues()
        
        result_peg_84.addResult(result_peg_97);
        
        result_peg_98 = Result(result_peg_84.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_98 = rule_whitespace(stream, result_peg_98.getPosition())
        if result_peg_98 == None:
            raise PegError
        
        result_peg_84.addResult(result_peg_98);
        
        result_peg_99 = Result(result_peg_84.getPosition())
        
        try:
            while True:
                result_peg_100 = Result(result_peg_99.getPosition());
                result_peg_101 = Result(result_peg_100.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_101 = rule_spaces(stream, result_peg_101.getPosition())
                if result_peg_101 == None:
                    raise PegError
                
                result_peg_100.addResult(result_peg_101);
                
                result_peg_102 = Result(result_peg_100.getPosition())
                
                if '|' == stream.get(result_peg_102.getPosition(), 1):
                    result_peg_102.nextPosition(1)
                    result_peg_102.setValue('|')
                else:
                    raise PegError
                
                result_peg_100.addResult(result_peg_102);
                
                result_peg_103 = Result(result_peg_100.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_103 = rule_spaces(stream, result_peg_103.getPosition())
                if result_peg_103 == None:
                    raise PegError
                
                result_peg_100.addResult(result_peg_103);
                
                result_peg_104 = Result(result_peg_100.getPosition())
                
                # print "Trying rule " + 'pattern_line'
                result_peg_104 = rule_pattern_line(stream, result_peg_104.getPosition())
                if result_peg_104 == None:
                    raise PegError
                pattern = result_peg_104.getValues()
                
                result_peg_100.addResult(result_peg_104);
                
                result_peg_105 = Result(result_peg_100.getPosition())
                
                # print "Trying rule " + 'whitespace'
                result_peg_105 = rule_whitespace(stream, result_peg_105.getPosition())
                if result_peg_105 == None:
                    raise PegError
                
                result_peg_100.addResult(result_peg_105);
                
                result_peg_106 = Result(result_peg_100.getPosition())
                
                value = None
                values = result_peg_100.getValues()
                value = pattern
                result_peg_106.setValue(value)
                
                result_peg_100.addResult(result_peg_106);
                
                result_peg_100.setValue(result_peg_100.getLastValue())
                result_peg_99.addResult(result_peg_100);
        except PegError:
            pass
        patterns = result_peg_99.getValues()
        
        result_peg_84.addResult(result_peg_99);
        
        result_peg_107 = Result(result_peg_84.getPosition())
        
        save_peg_108 = result_peg_107.getPosition()
        
        # print "Trying rule " + 'failure'
        result_peg_107 = rule_failure(stream, result_peg_107.getPosition())
        if result_peg_107 == None:
            
            result_peg_107 = Result(save_peg_108)
            result_peg_107.setValue(None)
        fail = result_peg_107.getValues()
        
        result_peg_84.addResult(result_peg_107);
        
        result_peg_109 = Result(result_peg_84.getPosition())
        
        value = None
        values = result_peg_84.getValues()
        value = peg.Rule(name, [pattern1] + patterns, inline = (inline != None), rules = rule_parameters, parameters = parameters, fail = fail)
        result_peg_109.setValue(value)
        
        result_peg_84.addResult(result_peg_109);
        
        result_peg_84.setValue(result_peg_84.getLastValue())
        stream.update(RULE_rule, position, result_peg_84)
        return result_peg_84
    except PegError:
        pass
    stream.update(RULE_rule, position, None)
    return None


def rule_pattern_line(stream, position):
    if stream.hasResult(RULE_pattern_line, position):
        return stream.result(RULE_pattern_line, position)
    try:
        result_peg_110 = Result(position)
        result_peg_111 = Result(result_peg_110.getPosition())
        
        try:
            while True:
                result_peg_112 = Result(result_peg_111.getPosition());
                # print "Trying rule " + 'pattern'
                result_peg_112 = rule_pattern(stream, result_peg_112.getPosition())
                if result_peg_112 == None:
                    raise PegError
                result_peg_111.addResult(result_peg_112);
        except PegError:
            pass
        patterns = result_peg_111.getValues()
        
        result_peg_110.addResult(result_peg_111);
        
        result_peg_113 = Result(result_peg_110.getPosition())
        
        value = None
        values = result_peg_110.getValues()
        value = peg.PatternSequence(patterns)
        #if code != None:
        #    value = code(peg.PatternSequence(patterns))
        #else:
        #    value = peg.PatternAction(peg.PatternSequence(patterns), "value = values;")
        result_peg_113.setValue(value)
        
        result_peg_110.addResult(result_peg_113);
        
        result_peg_110.setValue(result_peg_110.getLastValue())
        stream.update(RULE_pattern_line, position, result_peg_110)
        return result_peg_110
    except PegError:
        pass
    stream.update(RULE_pattern_line, position, None)
    return None


def rule_pattern(stream, position):
    if stream.hasResult(RULE_pattern, position):
        return stream.result(RULE_pattern, position)
    try:
        result_peg_114 = Result(position)
        result_peg_115 = Result(result_peg_114.getPosition())
        
        save_peg_116 = result_peg_115.getPosition()
        
        # print "Trying rule " + 'bind'
        result_peg_115 = rule_bind(stream, result_peg_115.getPosition())
        if result_peg_115 == None:
            
            result_peg_115 = Result(save_peg_116)
            result_peg_115.setValue(None)
        bind = result_peg_115.getValues()
        
        result_peg_114.addResult(result_peg_115);
        
        result_peg_117 = Result(result_peg_114.getPosition())
        
        # print "Trying rule " + 'item'
        result_peg_117 = rule_item(stream, result_peg_117.getPosition())
        if result_peg_117 == None:
            raise PegError
        item = result_peg_117.getValues()
        
        result_peg_114.addResult(result_peg_117);
        
        result_peg_118 = Result(result_peg_114.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_118 = rule_spaces(stream, result_peg_118.getPosition())
        if result_peg_118 == None:
            raise PegError
        
        result_peg_114.addResult(result_peg_118);
        
        result_peg_119 = Result(result_peg_114.getPosition())
        
        value = None
        values = result_peg_114.getValues()
        # value = peg.PatternRule(values[0])
        if bind != None:
            item = bind(item)
        value = item
        # print "Pattern is " + str(value)
        result_peg_119.setValue(value)
        
        result_peg_114.addResult(result_peg_119);
        
        result_peg_114.setValue(result_peg_114.getLastValue())
        stream.update(RULE_pattern, position, result_peg_114)
        return result_peg_114
    except PegError:
        pass
    stream.update(RULE_pattern, position, None)
    return None


def rule_raw_code(stream, position):
    if stream.hasResult(RULE_raw_code, position):
        return stream.result(RULE_raw_code, position)
    try:
        result_peg_120 = Result(position)
        result_peg_121 = Result(result_peg_120.getPosition())
        
        if '(' == stream.get(result_peg_121.getPosition(), 1):
            result_peg_121.nextPosition(1)
            result_peg_121.setValue('(')
        else:
            raise PegError
        
        result_peg_120.addResult(result_peg_121);
        
        result_peg_122 = Result(result_peg_120.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_122 = rule_spaces(stream, result_peg_122.getPosition())
        if result_peg_122 == None:
            raise PegError
        
        result_peg_120.addResult(result_peg_122);
        
        result_peg_123 = Result(result_peg_120.getPosition())
        
        # print "Trying rule " + 'code'
        result_peg_123 = rule_code(stream, result_peg_123.getPosition())
        if result_peg_123 == None:
            raise PegError
        code = result_peg_123.getValues()
        
        result_peg_120.addResult(result_peg_123);
        
        result_peg_124 = Result(result_peg_120.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_124 = rule_spaces(stream, result_peg_124.getPosition())
        if result_peg_124 == None:
            raise PegError
        
        result_peg_120.addResult(result_peg_124);
        
        result_peg_125 = Result(result_peg_120.getPosition())
        
        if ')' == stream.get(result_peg_125.getPosition(), 1):
            result_peg_125.nextPosition(1)
            result_peg_125.setValue(')')
        else:
            raise PegError
        
        result_peg_120.addResult(result_peg_125);
        
        result_peg_126 = Result(result_peg_120.getPosition())
        
        value = None
        values = result_peg_120.getValues()
        value = code.code
        result_peg_126.setValue(value)
        
        result_peg_120.addResult(result_peg_126);
        
        result_peg_120.setValue(result_peg_120.getLastValue())
        stream.update(RULE_raw_code, position, result_peg_120)
        return result_peg_120
    except PegError:
        pass
    stream.update(RULE_raw_code, position, None)
    return None


def rule_code(stream, position):
    if stream.hasResult(RULE_code, position):
        return stream.result(RULE_code, position)
    try:
        result_peg_127 = Result(position)
        result_peg_128 = Result(result_peg_127.getPosition())
        
        if '{{' == stream.get(result_peg_128.getPosition(), 2):
            result_peg_128.nextPosition(2)
            result_peg_128.setValue('{{')
        else:
            raise PegError
        
        result_peg_127.addResult(result_peg_128);
        
        result_peg_129 = Result(result_peg_127.getPosition())
        
        try:
            while True:
                result_peg_130 = Result(result_peg_129.getPosition());
                result_peg_132 = Result(result_peg_130.getPosition())
                
                result_peg_133 = Result(result_peg_132.getPosition());
                try:
                    if '}}' == stream.get(result_peg_133.getPosition(), 2):
                        result_peg_133.nextPosition(2)
                        result_peg_133.setValue('}}')
                    else:
                        raise NotError
                    raise PegError
                except NotError:
                    result_peg_132.setValue(None)
                        
                result_peg_130.addResult(result_peg_132);
                
                result_peg_134 = Result(result_peg_130.getPosition())
                
                temp_peg_135 = stream.get(result_peg_134.getPosition())
                if temp_peg_135 != chr(0):
                    result_peg_134.setValue(temp_peg_135)
                    result_peg_134.nextPosition()
                else:
                    raise PegError
                
                result_peg_130.addResult(result_peg_134);
                
                result_peg_136 = Result(result_peg_130.getPosition())
                
                value = None
                values = result_peg_130.getValues()
                value = values[1]
                result_peg_136.setValue(value)
                
                result_peg_130.addResult(result_peg_136);
                
                result_peg_130.setValue(result_peg_130.getLastValue())
                result_peg_129.addResult(result_peg_130);
        except PegError:
            if result_peg_129.matches() == 0:
                raise PegError
                
        result_peg_127.addResult(result_peg_129);
        
        result_peg_137 = Result(result_peg_127.getPosition())
        
        if '}}' == stream.get(result_peg_137.getPosition(), 2):
            result_peg_137.nextPosition(2)
            result_peg_137.setValue('}}')
        else:
            raise PegError
        
        result_peg_127.addResult(result_peg_137);
        
        result_peg_138 = Result(result_peg_127.getPosition())
        
        value = None
        values = result_peg_127.getValues()
        value = peg.PatternCode(''.join(values[1]))
        result_peg_138.setValue(value)
        
        result_peg_127.addResult(result_peg_138);
        
        result_peg_127.setValue(result_peg_127.getLastValue())
        stream.update(RULE_code, position, result_peg_127)
        return result_peg_127
    except PegError:
        pass
    stream.update(RULE_code, position, None)
    return None


def rule_item(stream, position):
    if stream.hasResult(RULE_item, position):
        return stream.result(RULE_item, position)
    try:
        result_peg_139 = Result(position)
        result_peg_140 = Result(result_peg_139.getPosition())
        
        save_peg_141 = result_peg_140.getPosition()
        
        if '&' == stream.get(result_peg_140.getPosition(), 1):
            result_peg_140.nextPosition(1)
            result_peg_140.setValue('&')
        else:
            
            result_peg_140 = Result(save_peg_141)
            result_peg_140.setValue(None)
        ensure = result_peg_140.getValues()
        
        result_peg_139.addResult(result_peg_140);
        
        result_peg_142 = Result(result_peg_139.getPosition())
        
        save_peg_143 = result_peg_142.getPosition()
        
        if '!' == stream.get(result_peg_142.getPosition(), 1):
            result_peg_142.nextPosition(1)
            result_peg_142.setValue('!')
        else:
            
            result_peg_142 = Result(save_peg_143)
            result_peg_142.setValue(None)
        pnot = result_peg_142.getValues()
        
        result_peg_139.addResult(result_peg_142);
        
        result_peg_144 = Result(result_peg_139.getPosition())
        
        save_peg_145 = result_peg_144.getPosition()
        
        result_peg_144 = Result(save_peg_145)
        # print "Trying rule " + 'x_word'
        result_peg_144 = rule_x_word(stream, result_peg_144.getPosition())
        if result_peg_144 == None:
            
            result_peg_144 = Result(save_peg_145)
            # print "Trying rule " + 'any'
            result_peg_144 = rule_any(stream, result_peg_144.getPosition())
            if result_peg_144 == None:
                
                result_peg_144 = Result(save_peg_145)
                # print "Trying rule " + 'eof'
                result_peg_144 = rule_eof(stream, result_peg_144.getPosition())
                if result_peg_144 == None:
                    
                    result_peg_144 = Result(save_peg_145)
                    # print "Trying rule " + 'void'
                    result_peg_144 = rule_void(stream, result_peg_144.getPosition())
                    if result_peg_144 == None:
                        
                        result_peg_144 = Result(save_peg_145)
                        # print "Trying rule " + 'range'
                        result_peg_144 = rule_range(stream, result_peg_144.getPosition())
                        if result_peg_144 == None:
                            
                            result_peg_144 = Result(save_peg_145)
                            # print "Trying rule " + 'string'
                            result_peg_144 = rule_string(stream, result_peg_144.getPosition())
                            if result_peg_144 == None:
                                
                                result_peg_144 = Result(save_peg_145)
                                # print "Trying rule " + 'line'
                                result_peg_144 = rule_line(stream, result_peg_144.getPosition())
                                if result_peg_144 == None:
                                    
                                    result_peg_144 = Result(save_peg_145)
                                    # print "Trying rule " + 'ascii'
                                    result_peg_144 = rule_ascii(stream, result_peg_144.getPosition())
                                    if result_peg_144 == None:
                                        
                                        result_peg_144 = Result(save_peg_145)
                                        # print "Trying rule " + 'utf8'
                                        result_peg_144 = rule_utf8(stream, result_peg_144.getPosition())
                                        if result_peg_144 == None:
                                            
                                            result_peg_144 = Result(save_peg_145)
                                            # print "Trying rule " + 'predicate'
                                            result_peg_144 = rule_predicate(stream, result_peg_144.getPosition())
                                            if result_peg_144 == None:
                                                
                                                result_peg_144 = Result(save_peg_145)
                                                # print "Trying rule " + 'call_rule'
                                                result_peg_144 = rule_call_rule(stream, result_peg_144.getPosition())
                                                if result_peg_144 == None:
                                                    
                                                    result_peg_144 = Result(save_peg_145)
                                                    # print "Trying rule " + 'sub_pattern'
                                                    result_peg_144 = rule_sub_pattern(stream, result_peg_144.getPosition())
                                                    if result_peg_144 == None:
                                                        
                                                        result_peg_144 = Result(save_peg_145)
                                                        # print "Trying rule " + 'code'
                                                        result_peg_144 = rule_code(stream, result_peg_144.getPosition())
                                                        if result_peg_144 == None:
                                                            raise PegError
        pattern = result_peg_144.getValues()
        
        result_peg_139.addResult(result_peg_144);
        
        result_peg_159 = Result(result_peg_139.getPosition())
        
        save_peg_160 = result_peg_159.getPosition()
        
        # print "Trying rule " + 'modifier'
        result_peg_159 = rule_modifier(stream, result_peg_159.getPosition())
        if result_peg_159 == None:
            
            result_peg_159 = Result(save_peg_160)
            result_peg_159.setValue(None)
        modifier = result_peg_159.getValues()
        
        result_peg_139.addResult(result_peg_159);
        
        result_peg_161 = Result(result_peg_139.getPosition())
        
        value = None
        values = result_peg_139.getValues()
        if modifier != None:
            pattern = modifier(pattern)
        if pnot != None:
            pattern = peg.PatternNot(pattern)
        if ensure != None:
            pattern = peg.PatternEnsure(pattern)
        value = pattern
        result_peg_161.setValue(value)
        
        result_peg_139.addResult(result_peg_161);
        
        result_peg_139.setValue(result_peg_139.getLastValue())
        stream.update(RULE_item, position, result_peg_139)
        return result_peg_139
    except PegError:
        pass
    stream.update(RULE_item, position, None)
    return None


def rule_failure(stream, position):
    if stream.hasResult(RULE_failure, position):
        return stream.result(RULE_failure, position)
    try:
        result_peg_162 = Result(position)
        result_peg_163 = Result(result_peg_162.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_163 = rule_whitespace(stream, result_peg_163.getPosition())
        if result_peg_163 == None:
            raise PegError
        
        result_peg_162.addResult(result_peg_163);
        
        result_peg_164 = Result(result_peg_162.getPosition())
        
        if '<fail>' == stream.get(result_peg_164.getPosition(), 6):
            result_peg_164.nextPosition(6)
            result_peg_164.setValue('<fail>')
        else:
            raise PegError
        
        result_peg_162.addResult(result_peg_164);
        
        result_peg_165 = Result(result_peg_162.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_165 = rule_spaces(stream, result_peg_165.getPosition())
        if result_peg_165 == None:
            raise PegError
        
        result_peg_162.addResult(result_peg_165);
        
        result_peg_166 = Result(result_peg_162.getPosition())
        
        # print "Trying rule " + 'code'
        result_peg_166 = rule_code(stream, result_peg_166.getPosition())
        if result_peg_166 == None:
            raise PegError
        code = result_peg_166.getValues()
        
        result_peg_162.addResult(result_peg_166);
        
        result_peg_167 = Result(result_peg_162.getPosition())
        
        value = None
        values = result_peg_162.getValues()
        value = code.code
        result_peg_167.setValue(value)
        
        result_peg_162.addResult(result_peg_167);
        
        result_peg_162.setValue(result_peg_162.getLastValue())
        stream.update(RULE_failure, position, result_peg_162)
        return result_peg_162
    except PegError:
        pass
    stream.update(RULE_failure, position, None)
    return None


def rule_line(stream, position):
    if stream.hasResult(RULE_line, position):
        return stream.result(RULE_line, position)
    try:
        result_peg_168 = Result(position)
        result_peg_169 = Result(result_peg_168.getPosition())
        
        if '<line>' == stream.get(result_peg_169.getPosition(), 6):
            result_peg_169.nextPosition(6)
            result_peg_169.setValue('<line>')
        else:
            raise PegError
        
        result_peg_168.addResult(result_peg_169);
        
        result_peg_170 = Result(result_peg_168.getPosition())
        
        value = None
        values = result_peg_168.getValues()
        value = peg.PatternLine()
        result_peg_170.setValue(value)
        
        result_peg_168.addResult(result_peg_170);
        
        result_peg_168.setValue(result_peg_168.getLastValue())
        stream.update(RULE_line, position, result_peg_168)
        return result_peg_168
    except PegError:
        pass
    stream.update(RULE_line, position, None)
    return None


def rule_predicate(stream, position):
    if stream.hasResult(RULE_predicate, position):
        return stream.result(RULE_predicate, position)
    try:
        result_peg_171 = Result(position)
        result_peg_172 = Result(result_peg_171.getPosition())
        
        if '<predicate' == stream.get(result_peg_172.getPosition(), 10):
            result_peg_172.nextPosition(10)
            result_peg_172.setValue('<predicate')
        else:
            raise PegError
        
        result_peg_171.addResult(result_peg_172);
        
        result_peg_173 = Result(result_peg_171.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_173 = rule_whitespace(stream, result_peg_173.getPosition())
        if result_peg_173 == None:
            raise PegError
        
        result_peg_171.addResult(result_peg_173);
        
        result_peg_174 = Result(result_peg_171.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_174 = rule_word(stream, result_peg_174.getPosition())
        if result_peg_174 == None:
            raise PegError
        variable = result_peg_174.getValues()
        
        result_peg_171.addResult(result_peg_174);
        
        result_peg_175 = Result(result_peg_171.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_175 = rule_whitespace(stream, result_peg_175.getPosition())
        if result_peg_175 == None:
            raise PegError
        
        result_peg_171.addResult(result_peg_175);
        
        result_peg_176 = Result(result_peg_171.getPosition())
        
        if '>' == stream.get(result_peg_176.getPosition(), 1):
            result_peg_176.nextPosition(1)
            result_peg_176.setValue('>')
        else:
            raise PegError
        
        result_peg_171.addResult(result_peg_176);
        
        result_peg_177 = Result(result_peg_171.getPosition())
        
        # print "Trying rule " + 'whitespace'
        result_peg_177 = rule_whitespace(stream, result_peg_177.getPosition())
        if result_peg_177 == None:
            raise PegError
        
        result_peg_171.addResult(result_peg_177);
        
        result_peg_178 = Result(result_peg_171.getPosition())
        
        # print "Trying rule " + 'code'
        result_peg_178 = rule_code(stream, result_peg_178.getPosition())
        if result_peg_178 == None:
            raise PegError
        code = result_peg_178.getValues()
        
        result_peg_171.addResult(result_peg_178);
        
        result_peg_179 = Result(result_peg_171.getPosition())
        
        value = None
        values = result_peg_171.getValues()
        value = peg.PatternPredicate(variable, code.code)
        result_peg_179.setValue(value)
        
        result_peg_171.addResult(result_peg_179);
        
        result_peg_171.setValue(result_peg_171.getLastValue())
        stream.update(RULE_predicate, position, result_peg_171)
        return result_peg_171
    except PegError:
        pass
    stream.update(RULE_predicate, position, None)
    return None


def rule_utf8(stream, position):
    if stream.hasResult(RULE_utf8, position):
        return stream.result(RULE_utf8, position)
    try:
        result_peg_180 = Result(position)
        result_peg_181 = Result(result_peg_180.getPosition())
        
        if '<utf8' == stream.get(result_peg_181.getPosition(), 5):
            result_peg_181.nextPosition(5)
            result_peg_181.setValue('<utf8')
        else:
            raise PegError
        
        result_peg_180.addResult(result_peg_181);
        
        result_peg_182 = Result(result_peg_180.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_182 = rule_spaces(stream, result_peg_182.getPosition())
        if result_peg_182 == None:
            raise PegError
        
        result_peg_180.addResult(result_peg_182);
        
        result_peg_183 = Result(result_peg_180.getPosition())
        
        # print "Trying rule " + 'hex_number'
        result_peg_183 = rule_hex_number(stream, result_peg_183.getPosition())
        if result_peg_183 == None:
            raise PegError
        num = result_peg_183.getValues()
        
        result_peg_180.addResult(result_peg_183);
        
        result_peg_184 = Result(result_peg_180.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_184 = rule_spaces(stream, result_peg_184.getPosition())
        if result_peg_184 == None:
            raise PegError
        
        result_peg_180.addResult(result_peg_184);
        
        result_peg_185 = Result(result_peg_180.getPosition())
        
        if '>' == stream.get(result_peg_185.getPosition(), 1):
            result_peg_185.nextPosition(1)
            result_peg_185.setValue('>')
        else:
            raise PegError
        
        result_peg_180.addResult(result_peg_185);
        
        result_peg_186 = Result(result_peg_180.getPosition())
        
        value = None
        values = result_peg_180.getValues()
        value = peg.createUtf8Pattern(num)
        result_peg_186.setValue(value)
        
        result_peg_180.addResult(result_peg_186);
        
        result_peg_180.setValue(result_peg_180.getLastValue())
        stream.update(RULE_utf8, position, result_peg_180)
        return result_peg_180
    except PegError:
        pass
    stream.update(RULE_utf8, position, None)
    return None


def rule_ascii(stream, position):
    if stream.hasResult(RULE_ascii, position):
        return stream.result(RULE_ascii, position)
    try:
        result_peg_187 = Result(position)
        result_peg_188 = Result(result_peg_187.getPosition())
        
        if '<ascii' == stream.get(result_peg_188.getPosition(), 6):
            result_peg_188.nextPosition(6)
            result_peg_188.setValue('<ascii')
        else:
            raise PegError
        
        result_peg_187.addResult(result_peg_188);
        
        result_peg_189 = Result(result_peg_187.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_189 = rule_spaces(stream, result_peg_189.getPosition())
        if result_peg_189 == None:
            raise PegError
        
        result_peg_187.addResult(result_peg_189);
        
        result_peg_190 = Result(result_peg_187.getPosition())
        
        # print "Trying rule " + 'number'
        result_peg_190 = rule_number(stream, result_peg_190.getPosition())
        if result_peg_190 == None:
            raise PegError
        num = result_peg_190.getValues()
        
        result_peg_187.addResult(result_peg_190);
        
        result_peg_191 = Result(result_peg_187.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_191 = rule_spaces(stream, result_peg_191.getPosition())
        if result_peg_191 == None:
            raise PegError
        
        result_peg_187.addResult(result_peg_191);
        
        result_peg_192 = Result(result_peg_187.getPosition())
        
        if '>' == stream.get(result_peg_192.getPosition(), 1):
            result_peg_192.nextPosition(1)
            result_peg_192.setValue('>')
        else:
            raise PegError
        
        result_peg_187.addResult(result_peg_192);
        
        result_peg_193 = Result(result_peg_187.getPosition())
        
        value = None
        values = result_peg_187.getValues()
        value = peg.PatternVerbatim(int(num))
        result_peg_193.setValue(value)
        
        result_peg_187.addResult(result_peg_193);
        
        result_peg_187.setValue(result_peg_187.getLastValue())
        stream.update(RULE_ascii, position, result_peg_187)
        return result_peg_187
    except PegError:
        pass
    stream.update(RULE_ascii, position, None)
    return None


def rule_call_rule(stream, position):
    if stream.hasResult(RULE_call_rule, position):
        return stream.result(RULE_call_rule, position)
    try:
        result_peg_194 = Result(position)
        result_peg_195 = Result(result_peg_194.getPosition())
        
        if '@' == stream.get(result_peg_195.getPosition(), 1):
            result_peg_195.nextPosition(1)
            result_peg_195.setValue('@')
        else:
            raise PegError
        
        result_peg_194.addResult(result_peg_195);
        
        result_peg_196 = Result(result_peg_194.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_196 = rule_word(stream, result_peg_196.getPosition())
        if result_peg_196 == None:
            raise PegError
        name = result_peg_196.getValues()
        
        result_peg_194.addResult(result_peg_196);
        
        result_peg_197 = Result(result_peg_194.getPosition())
        
        save_peg_198 = result_peg_197.getPosition()
        
        # print "Trying rule " + 'parameters_rules'
        result_peg_197 = rule_parameters_rules(stream, result_peg_197.getPosition())
        if result_peg_197 == None:
            
            result_peg_197 = Result(save_peg_198)
            result_peg_197.setValue(None)
        rule_parameters = result_peg_197.getValues()
        
        result_peg_194.addResult(result_peg_197);
        
        result_peg_199 = Result(result_peg_194.getPosition())
        
        save_peg_200 = result_peg_199.getPosition()
        
        # print "Trying rule " + 'parameters_values'
        result_peg_199 = rule_parameters_values(stream, result_peg_199.getPosition())
        if result_peg_199 == None:
            
            result_peg_199 = Result(save_peg_200)
            result_peg_199.setValue(None)
        parameters = result_peg_199.getValues()
        
        result_peg_194.addResult(result_peg_199);
        
        result_peg_201 = Result(result_peg_194.getPosition())
        
        value = None
        values = result_peg_194.getValues()
        value = peg.PatternCallRule(name, rule_parameters, parameters)
        result_peg_201.setValue(value)
        
        result_peg_194.addResult(result_peg_201);
        
        result_peg_194.setValue(result_peg_194.getLastValue())
        stream.update(RULE_call_rule, position, result_peg_194)
        return result_peg_194
    except PegError:
        pass
    stream.update(RULE_call_rule, position, None)
    return None


def rule_eof(stream, position):
    if stream.hasResult(RULE_eof, position):
        return stream.result(RULE_eof, position)
    try:
        result_peg_202 = Result(position)
        result_peg_203 = Result(result_peg_202.getPosition())
        
        if '<eof>' == stream.get(result_peg_203.getPosition(), 5):
            result_peg_203.nextPosition(5)
            result_peg_203.setValue('<eof>')
        else:
            raise PegError
        
        result_peg_202.addResult(result_peg_203);
        
        result_peg_204 = Result(result_peg_202.getPosition())
        
        value = None
        values = result_peg_202.getValues()
        value = peg.PatternEof()
        result_peg_204.setValue(value)
        
        result_peg_202.addResult(result_peg_204);
        
        result_peg_202.setValue(result_peg_202.getLastValue())
        stream.update(RULE_eof, position, result_peg_202)
        return result_peg_202
    except PegError:
        pass
    stream.update(RULE_eof, position, None)
    return None


def rule_void(stream, position):
    if stream.hasResult(RULE_void, position):
        return stream.result(RULE_void, position)
    try:
        result_peg_205 = Result(position)
        result_peg_206 = Result(result_peg_205.getPosition())
        
        if '<void>' == stream.get(result_peg_206.getPosition(), 6):
            result_peg_206.nextPosition(6)
            result_peg_206.setValue('<void>')
        else:
            raise PegError
        
        result_peg_205.addResult(result_peg_206);
        
        result_peg_207 = Result(result_peg_205.getPosition())
        
        value = None
        values = result_peg_205.getValues()
        value = peg.PatternVoid()
        result_peg_207.setValue(value)
        
        result_peg_205.addResult(result_peg_207);
        
        result_peg_205.setValue(result_peg_205.getLastValue())
        stream.update(RULE_void, position, result_peg_205)
        return result_peg_205
    except PegError:
        pass
    stream.update(RULE_void, position, None)
    return None


def rule_range(stream, position):
    if stream.hasResult(RULE_range, position):
        return stream.result(RULE_range, position)
    try:
        result_peg_208 = Result(position)
        result_peg_209 = Result(result_peg_208.getPosition())
        
        if '[' == stream.get(result_peg_209.getPosition(), 1):
            result_peg_209.nextPosition(1)
            result_peg_209.setValue('[')
        else:
            raise PegError
        
        result_peg_208.addResult(result_peg_209);
        
        result_peg_210 = Result(result_peg_208.getPosition())
        
        try:
            while True:
                result_peg_211 = Result(result_peg_210.getPosition());
                result_peg_212 = Result(result_peg_211.getPosition())
                
                result_peg_213 = Result(result_peg_212.getPosition());
                try:
                    if ']' == stream.get(result_peg_213.getPosition(), 1):
                        result_peg_213.nextPosition(1)
                        result_peg_213.setValue(']')
                    else:
                        raise NotError
                    raise PegError
                except NotError:
                    result_peg_212.setValue(None)
                        
                result_peg_211.addResult(result_peg_212);
                
                result_peg_214 = Result(result_peg_211.getPosition())
                
                temp_peg_215 = stream.get(result_peg_214.getPosition())
                if temp_peg_215 != chr(0):
                    result_peg_214.setValue(temp_peg_215)
                    result_peg_214.nextPosition()
                else:
                    raise PegError
                
                result_peg_211.addResult(result_peg_214);
                
                result_peg_216 = Result(result_peg_211.getPosition())
                
                value = None
                values = result_peg_211.getValues()
                value = values[1]
                result_peg_216.setValue(value)
                
                result_peg_211.addResult(result_peg_216);
                
                result_peg_211.setValue(result_peg_211.getLastValue())
                result_peg_210.addResult(result_peg_211);
        except PegError:
            pass
                
        result_peg_208.addResult(result_peg_210);
        
        result_peg_217 = Result(result_peg_208.getPosition())
        
        if ']' == stream.get(result_peg_217.getPosition(), 1):
            result_peg_217.nextPosition(1)
            result_peg_217.setValue(']')
        else:
            raise PegError
        
        result_peg_208.addResult(result_peg_217);
        
        result_peg_218 = Result(result_peg_208.getPosition())
        
        value = None
        values = result_peg_208.getValues()
        value = peg.PatternRange(''.join(values[1]))
        result_peg_218.setValue(value)
        
        result_peg_208.addResult(result_peg_218);
        
        result_peg_208.setValue(result_peg_208.getLastValue())
        stream.update(RULE_range, position, result_peg_208)
        return result_peg_208
    except PegError:
        pass
    stream.update(RULE_range, position, None)
    return None


def rule_sub_pattern(stream, position):
    if stream.hasResult(RULE_sub_pattern, position):
        return stream.result(RULE_sub_pattern, position)
    try:
        result_peg_219 = Result(position)
        result_peg_220 = Result(result_peg_219.getPosition())
        
        if '(' == stream.get(result_peg_220.getPosition(), 1):
            result_peg_220.nextPosition(1)
            result_peg_220.setValue('(')
        else:
            raise PegError
        
        result_peg_219.addResult(result_peg_220);
        
        result_peg_221 = Result(result_peg_219.getPosition())
        
        try:
            while True:
                result_peg_222 = Result(result_peg_221.getPosition());
                # print "Trying rule " + 'pattern'
                result_peg_222 = rule_pattern(stream, result_peg_222.getPosition())
                if result_peg_222 == None:
                    raise PegError
                result_peg_221.addResult(result_peg_222);
        except PegError:
            if result_peg_221.matches() == 0:
                raise PegError
                
        result_peg_219.addResult(result_peg_221);
        
        result_peg_224 = Result(result_peg_219.getPosition())
        
        if ')' == stream.get(result_peg_224.getPosition(), 1):
            result_peg_224.nextPosition(1)
            result_peg_224.setValue(')')
        else:
            raise PegError
        
        result_peg_219.addResult(result_peg_224);
        
        result_peg_225 = Result(result_peg_219.getPosition())
        
        value = None
        values = result_peg_219.getValues()
        value = peg.PatternSequence(values[1])
        result_peg_225.setValue(value)
        
        result_peg_219.addResult(result_peg_225);
        
        result_peg_219.setValue(result_peg_219.getLastValue())
        stream.update(RULE_sub_pattern, position, result_peg_219)
        return result_peg_219
    except PegError:
        pass
    stream.update(RULE_sub_pattern, position, None)
    return None


def rule_bind(stream, position):
    if stream.hasResult(RULE_bind, position):
        return stream.result(RULE_bind, position)
    try:
        result_peg_226 = Result(position)
        result_peg_227 = Result(result_peg_226.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_227 = rule_word(stream, result_peg_227.getPosition())
        if result_peg_227 == None:
            raise PegError
        name = result_peg_227.getValues()
        
        result_peg_226.addResult(result_peg_227);
        
        result_peg_228 = Result(result_peg_226.getPosition())
        
        if ':' == stream.get(result_peg_228.getPosition(), 1):
            result_peg_228.nextPosition(1)
            result_peg_228.setValue(':')
        else:
            raise PegError
        
        result_peg_226.addResult(result_peg_228);
        
        result_peg_229 = Result(result_peg_226.getPosition())
        
        value = None
        values = result_peg_226.getValues()
        value = lambda p: peg.PatternBind(name, p)
        result_peg_229.setValue(value)
        
        result_peg_226.addResult(result_peg_229);
        
        result_peg_226.setValue(result_peg_226.getLastValue())
        stream.update(RULE_bind, position, result_peg_226)
        return result_peg_226
    except PegError:
        pass
    stream.update(RULE_bind, position, None)
    return None


def rule_string(stream, position):
    if stream.hasResult(RULE_string, position):
        return stream.result(RULE_string, position)
    try:
        result_peg_230 = Result(position)
        result_peg_231 = Result(result_peg_230.getPosition())
        
        if '"' == stream.get(result_peg_231.getPosition(), 1):
            result_peg_231.nextPosition(1)
            result_peg_231.setValue('"')
        else:
            raise PegError
        
        result_peg_230.addResult(result_peg_231);
        
        result_peg_232 = Result(result_peg_230.getPosition())
        
        try:
            while True:
                result_peg_233 = Result(result_peg_232.getPosition());
                result_peg_234 = Result(result_peg_233.getPosition())
                
                result_peg_235 = Result(result_peg_234.getPosition());
                try:
                    if '"' == stream.get(result_peg_235.getPosition(), 1):
                        result_peg_235.nextPosition(1)
                        result_peg_235.setValue('"')
                    else:
                        raise NotError
                    raise PegError
                except NotError:
                    result_peg_234.setValue(None)
                        
                result_peg_233.addResult(result_peg_234);
                
                result_peg_236 = Result(result_peg_233.getPosition())
                
                temp_peg_237 = stream.get(result_peg_236.getPosition())
                if temp_peg_237 != chr(0):
                    result_peg_236.setValue(temp_peg_237)
                    result_peg_236.nextPosition()
                else:
                    raise PegError
                
                result_peg_233.addResult(result_peg_236);
                
                result_peg_238 = Result(result_peg_233.getPosition())
                
                value = None
                values = result_peg_233.getValues()
                value = values[1]
                result_peg_238.setValue(value)
                
                result_peg_233.addResult(result_peg_238);
                
                result_peg_233.setValue(result_peg_233.getLastValue())
                result_peg_232.addResult(result_peg_233);
        except PegError:
            pass
                
        result_peg_230.addResult(result_peg_232);
        
        result_peg_239 = Result(result_peg_230.getPosition())
        
        if '"' == stream.get(result_peg_239.getPosition(), 1):
            result_peg_239.nextPosition(1)
            result_peg_239.setValue('"')
        else:
            raise PegError
        
        result_peg_230.addResult(result_peg_239);
        
        result_peg_240 = Result(result_peg_230.getPosition())
        
        save_peg_241 = result_peg_240.getPosition()
        
        if '{case}' == stream.get(result_peg_240.getPosition(), 6):
            result_peg_240.nextPosition(6)
            result_peg_240.setValue('{case}')
        else:
            
            result_peg_240 = Result(save_peg_241)
            result_peg_240.setValue(None)
        options = result_peg_240.getValues()
        
        result_peg_230.addResult(result_peg_240);
        
        result_peg_242 = Result(result_peg_230.getPosition())
        
        value = None
        values = result_peg_230.getValues()
        value = peg.PatternVerbatim(''.join(values[1]), options)
        result_peg_242.setValue(value)
        
        result_peg_230.addResult(result_peg_242);
        
        result_peg_230.setValue(result_peg_230.getLastValue())
        stream.update(RULE_string, position, result_peg_230)
        return result_peg_230
    except PegError:
        pass
    try:
        result_peg_243 = Result(position)
        result_peg_244 = Result(result_peg_243.getPosition())
        
        if '<quote>' == stream.get(result_peg_244.getPosition(), 7):
            result_peg_244.nextPosition(7)
            result_peg_244.setValue('<quote>')
        else:
            raise PegError
        
        result_peg_243.addResult(result_peg_244);
        
        result_peg_245 = Result(result_peg_243.getPosition())
        
        value = None
        values = result_peg_243.getValues()
        value = peg.PatternVerbatim('"')
        result_peg_245.setValue(value)
        
        result_peg_243.addResult(result_peg_245);
        
        result_peg_243.setValue(result_peg_243.getLastValue())
        stream.update(RULE_string, position, result_peg_243)
        return result_peg_243
    except PegError:
        pass
    stream.update(RULE_string, position, None)
    return None


def rule_modifier(stream, position):
    if stream.hasResult(RULE_modifier, position):
        return stream.result(RULE_modifier, position)
    try:
        result_peg_246 = Result(position)
        result_peg_247 = Result(result_peg_246.getPosition())
        
        if '*' == stream.get(result_peg_247.getPosition(), 1):
            result_peg_247.nextPosition(1)
            result_peg_247.setValue('*')
        else:
            raise PegError
        
        result_peg_246.addResult(result_peg_247);
        
        result_peg_248 = Result(result_peg_246.getPosition())
        
        value = None
        values = result_peg_246.getValues()
        value = lambda p: peg.PatternRepeatMany(p)
        result_peg_248.setValue(value)
        
        result_peg_246.addResult(result_peg_248);
        
        result_peg_246.setValue(result_peg_246.getLastValue())
        stream.update(RULE_modifier, position, result_peg_246)
        return result_peg_246
    except PegError:
        pass
    try:
        result_peg_249 = Result(position)
        result_peg_250 = Result(result_peg_249.getPosition())
        
        if '?' == stream.get(result_peg_250.getPosition(), 1):
            result_peg_250.nextPosition(1)
            result_peg_250.setValue('?')
        else:
            raise PegError
        
        result_peg_249.addResult(result_peg_250);
        
        result_peg_251 = Result(result_peg_249.getPosition())
        
        value = None
        values = result_peg_249.getValues()
        value = lambda p: peg.PatternMaybe(p)
        result_peg_251.setValue(value)
        
        result_peg_249.addResult(result_peg_251);
        
        result_peg_249.setValue(result_peg_249.getLastValue())
        stream.update(RULE_modifier, position, result_peg_249)
        return result_peg_249
    except PegError:
        pass
    try:
        result_peg_252 = Result(position)
        result_peg_253 = Result(result_peg_252.getPosition())
        
        if '+' == stream.get(result_peg_253.getPosition(), 1):
            result_peg_253.nextPosition(1)
            result_peg_253.setValue('+')
        else:
            raise PegError
        
        result_peg_252.addResult(result_peg_253);
        
        result_peg_254 = Result(result_peg_252.getPosition())
        
        value = None
        values = result_peg_252.getValues()
        value = lambda p: peg.PatternRepeatOnce(p)
        result_peg_254.setValue(value)
        
        result_peg_252.addResult(result_peg_254);
        
        result_peg_252.setValue(result_peg_252.getLastValue())
        stream.update(RULE_modifier, position, result_peg_252)
        return result_peg_252
    except PegError:
        pass
    stream.update(RULE_modifier, position, None)
    return None


def rule_x_word(stream, position):
    if stream.hasResult(RULE_x_word, position):
        return stream.result(RULE_x_word, position)
    try:
        result_peg_255 = Result(position)
        result_peg_256 = Result(result_peg_255.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_256 = rule_word(stream, result_peg_256.getPosition())
        if result_peg_256 == None:
            raise PegError
        name = result_peg_256.getValues()
        
        result_peg_255.addResult(result_peg_256);
        
        result_peg_257 = Result(result_peg_255.getPosition())
        
        save_peg_258 = result_peg_257.getPosition()
        
        # print "Trying rule " + 'parameters_rules'
        result_peg_257 = rule_parameters_rules(stream, result_peg_257.getPosition())
        if result_peg_257 == None:
            
            result_peg_257 = Result(save_peg_258)
            result_peg_257.setValue(None)
        rule_parameters = result_peg_257.getValues()
        
        result_peg_255.addResult(result_peg_257);
        
        result_peg_259 = Result(result_peg_255.getPosition())
        
        save_peg_260 = result_peg_259.getPosition()
        
        # print "Trying rule " + 'parameters_values'
        result_peg_259 = rule_parameters_values(stream, result_peg_259.getPosition())
        if result_peg_259 == None:
            
            result_peg_259 = Result(save_peg_260)
            result_peg_259.setValue(None)
        parameters = result_peg_259.getValues()
        
        result_peg_255.addResult(result_peg_259);
        
        result_peg_261 = Result(result_peg_255.getPosition())
        
        value = None
        values = result_peg_255.getValues()
        value = peg.PatternRule(name, rule_parameters, parameters)
        result_peg_261.setValue(value)
        
        result_peg_255.addResult(result_peg_261);
        
        result_peg_255.setValue(result_peg_255.getLastValue())
        stream.update(RULE_x_word, position, result_peg_255)
        return result_peg_255
    except PegError:
        pass
    stream.update(RULE_x_word, position, None)
    return None


def rule_rule_parameters(stream, position):
    if stream.hasResult(RULE_rule_parameters, position):
        return stream.result(RULE_rule_parameters, position)
    try:
        result_peg_262 = Result(position)
        result_peg_263 = Result(result_peg_262.getPosition())
        
        if '[' == stream.get(result_peg_263.getPosition(), 1):
            result_peg_263.nextPosition(1)
            result_peg_263.setValue('[')
        else:
            raise PegError
        
        result_peg_262.addResult(result_peg_263);
        
        result_peg_264 = Result(result_peg_262.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_264 = rule_spaces(stream, result_peg_264.getPosition())
        if result_peg_264 == None:
            raise PegError
        
        result_peg_262.addResult(result_peg_264);
        
        result_peg_265 = Result(result_peg_262.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_265 = rule_word(stream, result_peg_265.getPosition())
        if result_peg_265 == None:
            raise PegError
        param1 = result_peg_265.getValues()
        
        result_peg_262.addResult(result_peg_265);
        
        result_peg_266 = Result(result_peg_262.getPosition())
        
        try:
            while True:
                result_peg_267 = Result(result_peg_266.getPosition());
                result_peg_268 = Result(result_peg_267.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_268 = rule_spaces(stream, result_peg_268.getPosition())
                if result_peg_268 == None:
                    raise PegError
                
                result_peg_267.addResult(result_peg_268);
                
                result_peg_269 = Result(result_peg_267.getPosition())
                
                if ',' == stream.get(result_peg_269.getPosition(), 1):
                    result_peg_269.nextPosition(1)
                    result_peg_269.setValue(',')
                else:
                    raise PegError
                
                result_peg_267.addResult(result_peg_269);
                
                result_peg_270 = Result(result_peg_267.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_270 = rule_spaces(stream, result_peg_270.getPosition())
                if result_peg_270 == None:
                    raise PegError
                
                result_peg_267.addResult(result_peg_270);
                
                result_peg_271 = Result(result_peg_267.getPosition())
                
                # print "Trying rule " + 'word'
                result_peg_271 = rule_word(stream, result_peg_271.getPosition())
                if result_peg_271 == None:
                    raise PegError
                exp = result_peg_271.getValues()
                
                result_peg_267.addResult(result_peg_271);
                
                result_peg_272 = Result(result_peg_267.getPosition())
                
                value = None
                values = result_peg_267.getValues()
                value = exp
                result_peg_272.setValue(value)
                
                result_peg_267.addResult(result_peg_272);
                
                result_peg_267.setValue(result_peg_267.getLastValue())
                result_peg_266.addResult(result_peg_267);
        except PegError:
            pass
        params = result_peg_266.getValues()
        
        result_peg_262.addResult(result_peg_266);
        
        result_peg_273 = Result(result_peg_262.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_273 = rule_spaces(stream, result_peg_273.getPosition())
        if result_peg_273 == None:
            raise PegError
        
        result_peg_262.addResult(result_peg_273);
        
        result_peg_274 = Result(result_peg_262.getPosition())
        
        if ']' == stream.get(result_peg_274.getPosition(), 1):
            result_peg_274.nextPosition(1)
            result_peg_274.setValue(']')
        else:
            raise PegError
        
        result_peg_262.addResult(result_peg_274);
        
        result_peg_275 = Result(result_peg_262.getPosition())
        
        value = None
        values = result_peg_262.getValues()
        value = [param1] + params
        result_peg_275.setValue(value)
        
        result_peg_262.addResult(result_peg_275);
        
        result_peg_262.setValue(result_peg_262.getLastValue())
        stream.update(RULE_rule_parameters, position, result_peg_262)
        return result_peg_262
    except PegError:
        pass
    stream.update(RULE_rule_parameters, position, None)
    return None


def rule_value_parameters(stream, position):
    if stream.hasResult(RULE_value_parameters, position):
        return stream.result(RULE_value_parameters, position)
    try:
        result_peg_276 = Result(position)
        result_peg_277 = Result(result_peg_276.getPosition())
        
        if '(' == stream.get(result_peg_277.getPosition(), 1):
            result_peg_277.nextPosition(1)
            result_peg_277.setValue('(')
        else:
            raise PegError
        
        result_peg_276.addResult(result_peg_277);
        
        result_peg_278 = Result(result_peg_276.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_278 = rule_spaces(stream, result_peg_278.getPosition())
        if result_peg_278 == None:
            raise PegError
        
        result_peg_276.addResult(result_peg_278);
        
        result_peg_279 = Result(result_peg_276.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_279 = rule_word(stream, result_peg_279.getPosition())
        if result_peg_279 == None:
            raise PegError
        param1 = result_peg_279.getValues()
        
        result_peg_276.addResult(result_peg_279);
        
        result_peg_280 = Result(result_peg_276.getPosition())
        
        try:
            while True:
                result_peg_281 = Result(result_peg_280.getPosition());
                result_peg_282 = Result(result_peg_281.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_282 = rule_spaces(stream, result_peg_282.getPosition())
                if result_peg_282 == None:
                    raise PegError
                
                result_peg_281.addResult(result_peg_282);
                
                result_peg_283 = Result(result_peg_281.getPosition())
                
                if ',' == stream.get(result_peg_283.getPosition(), 1):
                    result_peg_283.nextPosition(1)
                    result_peg_283.setValue(',')
                else:
                    raise PegError
                
                result_peg_281.addResult(result_peg_283);
                
                result_peg_284 = Result(result_peg_281.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_284 = rule_spaces(stream, result_peg_284.getPosition())
                if result_peg_284 == None:
                    raise PegError
                
                result_peg_281.addResult(result_peg_284);
                
                result_peg_285 = Result(result_peg_281.getPosition())
                
                # print "Trying rule " + 'word'
                result_peg_285 = rule_word(stream, result_peg_285.getPosition())
                if result_peg_285 == None:
                    raise PegError
                exp = result_peg_285.getValues()
                
                result_peg_281.addResult(result_peg_285);
                
                result_peg_286 = Result(result_peg_281.getPosition())
                
                value = None
                values = result_peg_281.getValues()
                value = exp
                result_peg_286.setValue(value)
                
                result_peg_281.addResult(result_peg_286);
                
                result_peg_281.setValue(result_peg_281.getLastValue())
                result_peg_280.addResult(result_peg_281);
        except PegError:
            pass
        params = result_peg_280.getValues()
        
        result_peg_276.addResult(result_peg_280);
        
        result_peg_287 = Result(result_peg_276.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_287 = rule_spaces(stream, result_peg_287.getPosition())
        if result_peg_287 == None:
            raise PegError
        
        result_peg_276.addResult(result_peg_287);
        
        result_peg_288 = Result(result_peg_276.getPosition())
        
        if ')' == stream.get(result_peg_288.getPosition(), 1):
            result_peg_288.nextPosition(1)
            result_peg_288.setValue(')')
        else:
            raise PegError
        
        result_peg_276.addResult(result_peg_288);
        
        result_peg_289 = Result(result_peg_276.getPosition())
        
        value = None
        values = result_peg_276.getValues()
        value = [param1] + params
        result_peg_289.setValue(value)
        
        result_peg_276.addResult(result_peg_289);
        
        result_peg_276.setValue(result_peg_276.getLastValue())
        stream.update(RULE_value_parameters, position, result_peg_276)
        return result_peg_276
    except PegError:
        pass
    stream.update(RULE_value_parameters, position, None)
    return None


def rule_parameters_rules(stream, position):
    if stream.hasResult(RULE_parameters_rules, position):
        return stream.result(RULE_parameters_rules, position)
    try:
        result_peg_290 = Result(position)
        result_peg_291 = Result(result_peg_290.getPosition())
        
        if '[' == stream.get(result_peg_291.getPosition(), 1):
            result_peg_291.nextPosition(1)
            result_peg_291.setValue('[')
        else:
            raise PegError
        
        result_peg_290.addResult(result_peg_291);
        
        result_peg_292 = Result(result_peg_290.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_292 = rule_spaces(stream, result_peg_292.getPosition())
        if result_peg_292 == None:
            raise PegError
        
        result_peg_290.addResult(result_peg_292);
        
        result_peg_293 = Result(result_peg_290.getPosition())
        
        # print "Trying rule " + 'word_or_at'
        result_peg_293 = rule_word_or_at(stream, result_peg_293.getPosition())
        if result_peg_293 == None:
            raise PegError
        param1 = result_peg_293.getValues()
        
        result_peg_290.addResult(result_peg_293);
        
        result_peg_294 = Result(result_peg_290.getPosition())
        
        try:
            while True:
                result_peg_295 = Result(result_peg_294.getPosition());
                result_peg_296 = Result(result_peg_295.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_296 = rule_spaces(stream, result_peg_296.getPosition())
                if result_peg_296 == None:
                    raise PegError
                
                result_peg_295.addResult(result_peg_296);
                
                result_peg_297 = Result(result_peg_295.getPosition())
                
                if ',' == stream.get(result_peg_297.getPosition(), 1):
                    result_peg_297.nextPosition(1)
                    result_peg_297.setValue(',')
                else:
                    raise PegError
                
                result_peg_295.addResult(result_peg_297);
                
                result_peg_298 = Result(result_peg_295.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_298 = rule_spaces(stream, result_peg_298.getPosition())
                if result_peg_298 == None:
                    raise PegError
                
                result_peg_295.addResult(result_peg_298);
                
                result_peg_299 = Result(result_peg_295.getPosition())
                
                # print "Trying rule " + 'word_or_at'
                result_peg_299 = rule_word_or_at(stream, result_peg_299.getPosition())
                if result_peg_299 == None:
                    raise PegError
                exp = result_peg_299.getValues()
                
                result_peg_295.addResult(result_peg_299);
                
                result_peg_300 = Result(result_peg_295.getPosition())
                
                value = None
                values = result_peg_295.getValues()
                value = exp
                result_peg_300.setValue(value)
                
                result_peg_295.addResult(result_peg_300);
                
                result_peg_295.setValue(result_peg_295.getLastValue())
                result_peg_294.addResult(result_peg_295);
        except PegError:
            pass
        params = result_peg_294.getValues()
        
        result_peg_290.addResult(result_peg_294);
        
        result_peg_301 = Result(result_peg_290.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_301 = rule_spaces(stream, result_peg_301.getPosition())
        if result_peg_301 == None:
            raise PegError
        
        result_peg_290.addResult(result_peg_301);
        
        result_peg_302 = Result(result_peg_290.getPosition())
        
        if ']' == stream.get(result_peg_302.getPosition(), 1):
            result_peg_302.nextPosition(1)
            result_peg_302.setValue(']')
        else:
            raise PegError
        
        result_peg_290.addResult(result_peg_302);
        
        result_peg_303 = Result(result_peg_290.getPosition())
        
        value = None
        values = result_peg_290.getValues()
        value = [param1] + params
        result_peg_303.setValue(value)
        
        result_peg_290.addResult(result_peg_303);
        
        result_peg_290.setValue(result_peg_290.getLastValue())
        stream.update(RULE_parameters_rules, position, result_peg_290)
        return result_peg_290
    except PegError:
        pass
    stream.update(RULE_parameters_rules, position, None)
    return None


def rule_parameters_values(stream, position):
    if stream.hasResult(RULE_parameters_values, position):
        return stream.result(RULE_parameters_values, position)
    try:
        result_peg_304 = Result(position)
        result_peg_305 = Result(result_peg_304.getPosition())
        
        if '(' == stream.get(result_peg_305.getPosition(), 1):
            result_peg_305.nextPosition(1)
            result_peg_305.setValue('(')
        else:
            raise PegError
        
        result_peg_304.addResult(result_peg_305);
        
        result_peg_306 = Result(result_peg_304.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_306 = rule_spaces(stream, result_peg_306.getPosition())
        if result_peg_306 == None:
            raise PegError
        
        result_peg_304.addResult(result_peg_306);
        
        result_peg_307 = Result(result_peg_304.getPosition())
        
        # print "Trying rule " + 'word_or_dollar'
        result_peg_307 = rule_word_or_dollar(stream, result_peg_307.getPosition())
        if result_peg_307 == None:
            raise PegError
        param1 = result_peg_307.getValues()
        
        result_peg_304.addResult(result_peg_307);
        
        result_peg_308 = Result(result_peg_304.getPosition())
        
        try:
            while True:
                result_peg_309 = Result(result_peg_308.getPosition());
                result_peg_310 = Result(result_peg_309.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_310 = rule_spaces(stream, result_peg_310.getPosition())
                if result_peg_310 == None:
                    raise PegError
                
                result_peg_309.addResult(result_peg_310);
                
                result_peg_311 = Result(result_peg_309.getPosition())
                
                if ',' == stream.get(result_peg_311.getPosition(), 1):
                    result_peg_311.nextPosition(1)
                    result_peg_311.setValue(',')
                else:
                    raise PegError
                
                result_peg_309.addResult(result_peg_311);
                
                result_peg_312 = Result(result_peg_309.getPosition())
                
                # print "Trying rule " + 'spaces'
                result_peg_312 = rule_spaces(stream, result_peg_312.getPosition())
                if result_peg_312 == None:
                    raise PegError
                
                result_peg_309.addResult(result_peg_312);
                
                result_peg_313 = Result(result_peg_309.getPosition())
                
                # print "Trying rule " + 'word_or_dollar'
                result_peg_313 = rule_word_or_dollar(stream, result_peg_313.getPosition())
                if result_peg_313 == None:
                    raise PegError
                exp = result_peg_313.getValues()
                
                result_peg_309.addResult(result_peg_313);
                
                result_peg_314 = Result(result_peg_309.getPosition())
                
                value = None
                values = result_peg_309.getValues()
                value = exp
                result_peg_314.setValue(value)
                
                result_peg_309.addResult(result_peg_314);
                
                result_peg_309.setValue(result_peg_309.getLastValue())
                result_peg_308.addResult(result_peg_309);
        except PegError:
            pass
        params = result_peg_308.getValues()
        
        result_peg_304.addResult(result_peg_308);
        
        result_peg_315 = Result(result_peg_304.getPosition())
        
        # print "Trying rule " + 'spaces'
        result_peg_315 = rule_spaces(stream, result_peg_315.getPosition())
        if result_peg_315 == None:
            raise PegError
        
        result_peg_304.addResult(result_peg_315);
        
        result_peg_316 = Result(result_peg_304.getPosition())
        
        if ')' == stream.get(result_peg_316.getPosition(), 1):
            result_peg_316.nextPosition(1)
            result_peg_316.setValue(')')
        else:
            raise PegError
        
        result_peg_304.addResult(result_peg_316);
        
        result_peg_317 = Result(result_peg_304.getPosition())
        
        value = None
        values = result_peg_304.getValues()
        value = [param1] + params
        result_peg_317.setValue(value)
        
        result_peg_304.addResult(result_peg_317);
        
        result_peg_304.setValue(result_peg_304.getLastValue())
        stream.update(RULE_parameters_values, position, result_peg_304)
        return result_peg_304
    except PegError:
        pass
    stream.update(RULE_parameters_values, position, None)
    return None


def rule_word_or_dollar(stream, position):
    if stream.hasResult(RULE_word_or_dollar, position):
        return stream.result(RULE_word_or_dollar, position)
    try:
        result_peg_318 = Result(position)
        # print "Trying rule " + 'word'
        result_peg_318 = rule_word(stream, result_peg_318.getPosition())
        if result_peg_318 == None:
            raise PegError
        stream.update(RULE_word_or_dollar, position, result_peg_318)
        return result_peg_318
    except PegError:
        pass
    try:
        result_peg_319 = Result(position)
        # print "Trying rule " + 'dollar'
        result_peg_319 = rule_dollar(stream, result_peg_319.getPosition())
        if result_peg_319 == None:
            raise PegError
        stream.update(RULE_word_or_dollar, position, result_peg_319)
        return result_peg_319
    except PegError:
        pass
    stream.update(RULE_word_or_dollar, position, None)
    return None


def rule_word_or_at(stream, position):
    if stream.hasResult(RULE_word_or_at, position):
        return stream.result(RULE_word_or_at, position)
    try:
        result_peg_320 = Result(position)
        # print "Trying rule " + 'word'
        result_peg_320 = rule_word(stream, result_peg_320.getPosition())
        if result_peg_320 == None:
            raise PegError
        stream.update(RULE_word_or_at, position, result_peg_320)
        return result_peg_320
    except PegError:
        pass
    try:
        result_peg_321 = Result(position)
        # print "Trying rule " + 'word_at'
        result_peg_321 = rule_word_at(stream, result_peg_321.getPosition())
        if result_peg_321 == None:
            raise PegError
        stream.update(RULE_word_or_at, position, result_peg_321)
        return result_peg_321
    except PegError:
        pass
    stream.update(RULE_word_or_at, position, None)
    return None


def rule_word_at(stream, position):
    if stream.hasResult(RULE_word_at, position):
        return stream.result(RULE_word_at, position)
    try:
        result_peg_322 = Result(position)
        result_peg_323 = Result(result_peg_322.getPosition())
        
        if '@' == stream.get(result_peg_323.getPosition(), 1):
            result_peg_323.nextPosition(1)
            result_peg_323.setValue('@')
        else:
            raise PegError
        
        result_peg_322.addResult(result_peg_323);
        
        result_peg_324 = Result(result_peg_322.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_324 = rule_word(stream, result_peg_324.getPosition())
        if result_peg_324 == None:
            raise PegError
        word = result_peg_324.getValues()
        
        result_peg_322.addResult(result_peg_324);
        
        result_peg_325 = Result(result_peg_322.getPosition())
        
        value = None
        values = result_peg_322.getValues()
        value = '@%s' % word
        result_peg_325.setValue(value)
        
        result_peg_322.addResult(result_peg_325);
        
        result_peg_322.setValue(result_peg_322.getLastValue())
        stream.update(RULE_word_at, position, result_peg_322)
        return result_peg_322
    except PegError:
        pass
    stream.update(RULE_word_at, position, None)
    return None


def rule_dollar(stream, position):
    if stream.hasResult(RULE_dollar, position):
        return stream.result(RULE_dollar, position)
    try:
        result_peg_326 = Result(position)
        result_peg_327 = Result(result_peg_326.getPosition())
        
        if '$' == stream.get(result_peg_327.getPosition(), 1):
            result_peg_327.nextPosition(1)
            result_peg_327.setValue('$')
        else:
            raise PegError
        
        result_peg_326.addResult(result_peg_327);
        
        result_peg_328 = Result(result_peg_326.getPosition())
        
        # print "Trying rule " + 'number'
        result_peg_328 = rule_number(stream, result_peg_328.getPosition())
        if result_peg_328 == None:
            raise PegError
        number = result_peg_328.getValues()
        
        result_peg_326.addResult(result_peg_328);
        
        result_peg_329 = Result(result_peg_326.getPosition())
        
        value = None
        values = result_peg_326.getValues()
        value = "$%s" % number
        result_peg_329.setValue(value)
        
        result_peg_326.addResult(result_peg_329);
        
        result_peg_326.setValue(result_peg_326.getLastValue())
        stream.update(RULE_dollar, position, result_peg_326)
        return result_peg_326
    except PegError:
        pass
    stream.update(RULE_dollar, position, None)
    return None


def rule_number(stream, position):
    if stream.hasResult(RULE_number, position):
        return stream.result(RULE_number, position)
    try:
        result_peg_330 = Result(position)
        result_peg_331 = Result(result_peg_330.getPosition())
        
        try:
            while True:
                result_peg_332 = Result(result_peg_331.getPosition());
                # print "Trying rule " + 'digit'
                result_peg_332 = rule_digit(stream, result_peg_332.getPosition())
                if result_peg_332 == None:
                    raise PegError
                result_peg_331.addResult(result_peg_332);
        except PegError:
            if result_peg_331.matches() == 0:
                raise PegError
                
        result_peg_330.addResult(result_peg_331);
        
        result_peg_334 = Result(result_peg_330.getPosition())
        
        value = None
        values = result_peg_330.getValues()
        value = ''.join(values[0])
        result_peg_334.setValue(value)
        
        result_peg_330.addResult(result_peg_334);
        
        result_peg_330.setValue(result_peg_330.getLastValue())
        stream.update(RULE_number, position, result_peg_330)
        return result_peg_330
    except PegError:
        pass
    stream.update(RULE_number, position, None)
    return None


def rule_digit(stream, position):
    if stream.hasResult(RULE_digit, position):
        return stream.result(RULE_digit, position)
    try:
        result_peg_335 = Result(position)
        letter_peg_336 = stream.get(result_peg_335.getPosition())
        if letter_peg_336 in '0123456789':
            result_peg_335.nextPosition()
            result_peg_335.setValue(letter_peg_336)
        else:
            raise PegError
        stream.update(RULE_digit, position, result_peg_335)
        return result_peg_335
    except PegError:
        pass
    stream.update(RULE_digit, position, None)
    return None


def rule_hex_number(stream, position):
    if stream.hasResult(RULE_hex_number, position):
        return stream.result(RULE_hex_number, position)
    try:
        result_peg_337 = Result(position)
        result_peg_338 = Result(result_peg_337.getPosition())
        
        try:
            while True:
                result_peg_339 = Result(result_peg_338.getPosition());
                # print "Trying rule " + 'hex_digit'
                result_peg_339 = rule_hex_digit(stream, result_peg_339.getPosition())
                if result_peg_339 == None:
                    raise PegError
                result_peg_338.addResult(result_peg_339);
        except PegError:
            if result_peg_338.matches() == 0:
                raise PegError
                
        result_peg_337.addResult(result_peg_338);
        
        result_peg_341 = Result(result_peg_337.getPosition())
        
        value = None
        values = result_peg_337.getValues()
        value = ''.join(values[0])
        result_peg_341.setValue(value)
        
        result_peg_337.addResult(result_peg_341);
        
        result_peg_337.setValue(result_peg_337.getLastValue())
        stream.update(RULE_hex_number, position, result_peg_337)
        return result_peg_337
    except PegError:
        pass
    stream.update(RULE_hex_number, position, None)
    return None


def rule_hex_digit(stream, position):
    if stream.hasResult(RULE_hex_digit, position):
        return stream.result(RULE_hex_digit, position)
    try:
        result_peg_342 = Result(position)
        letter_peg_343 = stream.get(result_peg_342.getPosition())
        if letter_peg_343 in '0123456789abcdefABCDEF':
            result_peg_342.nextPosition()
            result_peg_342.setValue(letter_peg_343)
        else:
            raise PegError
        stream.update(RULE_hex_digit, position, result_peg_342)
        return result_peg_342
    except PegError:
        pass
    stream.update(RULE_hex_digit, position, None)
    return None


def rule_start_symbol(stream, position):
    if stream.hasResult(RULE_start_symbol, position):
        return stream.result(RULE_start_symbol, position)
    try:
        result_peg_344 = Result(position)
        result_peg_345 = Result(result_peg_344.getPosition())
        
        if 'start-symbol:' == stream.get(result_peg_345.getPosition(), 13):
            result_peg_345.nextPosition(13)
            result_peg_345.setValue('start-symbol:')
        else:
            raise PegError
        
        result_peg_344.addResult(result_peg_345);
        
        result_peg_346 = Result(result_peg_344.getPosition())
        
        try:
            while True:
                result_peg_347 = Result(result_peg_346.getPosition());
                # print "Trying rule " + 'space'
                result_peg_347 = rule_space(stream, result_peg_347.getPosition())
                if result_peg_347 == None:
                    raise PegError
                result_peg_346.addResult(result_peg_347);
        except PegError:
            pass
                
        result_peg_344.addResult(result_peg_346);
        
        result_peg_348 = Result(result_peg_344.getPosition())
        
        # print "Trying rule " + 'word'
        result_peg_348 = rule_word(stream, result_peg_348.getPosition())
        if result_peg_348 == None:
            raise PegError
        
        result_peg_344.addResult(result_peg_348);
        
        result_peg_349 = Result(result_peg_344.getPosition())
        
        value = None
        values = result_peg_344.getValues()
        value = values[2]
        result_peg_349.setValue(value)
        
        result_peg_344.addResult(result_peg_349);
        
        result_peg_344.setValue(result_peg_344.getLastValue())
        stream.update(RULE_start_symbol, position, result_peg_344)
        return result_peg_344
    except PegError:
        pass
    stream.update(RULE_start_symbol, position, None)
    return None


def rule_spaces(stream, position):
    if stream.hasResult(RULE_spaces, position):
        return stream.result(RULE_spaces, position)
    try:
        result_peg_350 = Result(position)
        result_peg_351 = Result(result_peg_350.getPosition())
        
        try:
            while True:
                result_peg_352 = Result(result_peg_351.getPosition());
                # print "Trying rule " + 'space'
                result_peg_352 = rule_space(stream, result_peg_352.getPosition())
                if result_peg_352 == None:
                    raise PegError
                result_peg_351.addResult(result_peg_352);
        except PegError:
            pass
                
        result_peg_350.addResult(result_peg_351);
        
        result_peg_350.setValue(result_peg_350.getLastValue())
        stream.update(RULE_spaces, position, result_peg_350)
        return result_peg_350
    except PegError:
        pass
    stream.update(RULE_spaces, position, None)
    return None


def rule_space(stream, position):
    if stream.hasResult(RULE_space, position):
        return stream.result(RULE_space, position)
    try:
        result_peg_353 = Result(position)
        result_peg_354 = Result(result_peg_353.getPosition())
        
        if ' ' == stream.get(result_peg_354.getPosition(), 1):
            result_peg_354.nextPosition(1)
            result_peg_354.setValue(' ')
        else:
            raise PegError
        
        result_peg_353.addResult(result_peg_354);
        
        result_peg_353.setValue(result_peg_353.getLastValue())
        stream.update(RULE_space, position, result_peg_353)
        return result_peg_353
    except PegError:
        pass
    try:
        result_peg_355 = Result(position)
        result_peg_356 = Result(result_peg_355.getPosition())
        
        if '\t' == stream.get(result_peg_356.getPosition(), 1):
            result_peg_356.nextPosition(1)
            result_peg_356.setValue('\t')
        else:
            raise PegError
        
        result_peg_355.addResult(result_peg_356);
        
        result_peg_355.setValue(result_peg_355.getLastValue())
        stream.update(RULE_space, position, result_peg_355)
        return result_peg_355
    except PegError:
        pass
    stream.update(RULE_space, position, None)
    return None


def rule_any_char(stream, position):
    if stream.hasResult(RULE_any_char, position):
        return stream.result(RULE_any_char, position)
    try:
        result_peg_357 = Result(position)
        letter_peg_358 = stream.get(result_peg_357.getPosition())
        if letter_peg_358 in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-':
            result_peg_357.nextPosition()
            result_peg_357.setValue(letter_peg_358)
        else:
            raise PegError
        stream.update(RULE_any_char, position, result_peg_357)
        return result_peg_357
    except PegError:
        pass
    stream.update(RULE_any_char, position, None)
    return None


def rule_any(stream, position):
    if stream.hasResult(RULE_any, position):
        return stream.result(RULE_any, position)
    try:
        result_peg_359 = Result(position)
        result_peg_360 = Result(result_peg_359.getPosition())
        
        if '.' == stream.get(result_peg_360.getPosition(), 1):
            result_peg_360.nextPosition(1)
            result_peg_360.setValue('.')
        else:
            raise PegError
        
        result_peg_359.addResult(result_peg_360);
        
        result_peg_361 = Result(result_peg_359.getPosition())
        
        value = None
        values = result_peg_359.getValues()
        value = peg.PatternAny()
        result_peg_361.setValue(value)
        
        result_peg_359.addResult(result_peg_361);
        
        result_peg_359.setValue(result_peg_359.getLastValue())
        stream.update(RULE_any, position, result_peg_359)
        return result_peg_359
    except PegError:
        pass
    stream.update(RULE_any, position, None)
    return None


def rule_whitespace(stream, position):
    if stream.hasResult(RULE_whitespace, position):
        return stream.result(RULE_whitespace, position)
    try:
        result_peg_362 = Result(position)
        result_peg_363 = Result(result_peg_362.getPosition())
        
        try:
            while True:
                result_peg_364 = Result(result_peg_363.getPosition());
                save_peg_365 = result_peg_364.getPosition()
                
                result_peg_364 = Result(save_peg_365)
                letter_peg_368 = stream.get(result_peg_364.getPosition())
                if letter_peg_368 in ' \t\n':
                    result_peg_364.nextPosition()
                    result_peg_364.setValue(letter_peg_368)
                else:
                    
                    result_peg_364 = Result(save_peg_365)
                    # print "Trying rule " + 'comment'
                    result_peg_364 = rule_comment(stream, result_peg_364.getPosition())
                    if result_peg_364 == None:
                        raise PegError
                result_peg_363.addResult(result_peg_364);
        except PegError:
            pass
                
        result_peg_362.addResult(result_peg_363);
        
        result_peg_362.setValue(result_peg_362.getLastValue())
        stream.update(RULE_whitespace, position, result_peg_362)
        return result_peg_362
    except PegError:
        pass
    stream.update(RULE_whitespace, position, None)
    return None


def rule_comment(stream, position):
    if stream.hasResult(RULE_comment, position):
        return stream.result(RULE_comment, position)
    try:
        result_peg_369 = Result(position)
        result_peg_370 = Result(result_peg_369.getPosition())
        
        if '#' == stream.get(result_peg_370.getPosition(), 1):
            result_peg_370.nextPosition(1)
            result_peg_370.setValue('#')
        else:
            raise PegError
        
        result_peg_369.addResult(result_peg_370);
        
        result_peg_371 = Result(result_peg_369.getPosition())
        
        try:
            while True:
                result_peg_372 = Result(result_peg_371.getPosition());
                result_peg_373 = Result(result_peg_372.getPosition())
                
                result_peg_374 = Result(result_peg_373.getPosition());
                try:
                    if '\n' == stream.get(result_peg_374.getPosition(), 1):
                        result_peg_374.nextPosition(1)
                        result_peg_374.setValue('\n')
                    else:
                        raise NotError
                    raise PegError
                except NotError:
                    result_peg_373.setValue(None)
                        
                result_peg_372.addResult(result_peg_373);
                
                result_peg_375 = Result(result_peg_372.getPosition())
                
                temp_peg_376 = stream.get(result_peg_375.getPosition())
                if temp_peg_376 != chr(0):
                    result_peg_375.setValue(temp_peg_376)
                    result_peg_375.nextPosition()
                else:
                    raise PegError
                
                result_peg_372.addResult(result_peg_375);
                
                result_peg_372.setValue(result_peg_372.getLastValue())
                result_peg_371.addResult(result_peg_372);
        except PegError:
            pass
                
        result_peg_369.addResult(result_peg_371);
        
        result_peg_369.setValue(result_peg_369.getLastValue())
        stream.update(RULE_comment, position, result_peg_369)
        return result_peg_369
    except PegError:
        pass
    stream.update(RULE_comment, position, None)
    return None


def rule_newlines_one(stream, position):
    if stream.hasResult(RULE_newlines_one, position):
        return stream.result(RULE_newlines_one, position)
    try:
        result_peg_377 = Result(position)
        try:
            while True:
                result_peg_378 = Result(result_peg_377.getPosition());
                if '\n' == stream.get(result_peg_378.getPosition(), 1):
                    result_peg_378.nextPosition(1)
                    result_peg_378.setValue('\n')
                else:
                    raise PegError
                result_peg_377.addResult(result_peg_378);
        except PegError:
            if result_peg_377.matches() == 0:
                raise PegError
        stream.update(RULE_newlines_one, position, result_peg_377)
        return result_peg_377
    except PegError:
        pass
    stream.update(RULE_newlines_one, position, None)
    return None


def rule_newlines(stream, position):
    if stream.hasResult(RULE_newlines, position):
        return stream.result(RULE_newlines, position)
    try:
        result_peg_380 = Result(position)
        try:
            while True:
                result_peg_381 = Result(result_peg_380.getPosition());
                if '\n' == stream.get(result_peg_381.getPosition(), 1):
                    result_peg_381.nextPosition(1)
                    result_peg_381.setValue('\n')
                else:
                    raise PegError
                result_peg_380.addResult(result_peg_381);
        except PegError:
            pass
        stream.update(RULE_newlines, position, result_peg_380)
        return result_peg_380
    except PegError:
        pass
    stream.update(RULE_newlines, position, None)
    return None


def doParse(stream):
    done = rule_start(stream, 0)
    if done == None:
        # print "Error parsing " + file
        stream.reportError()
        return None
    else:
        return done.getValues()

def parseFile(file):
    # print "Parsing " + file
    return doParse(Stream(filename = file))

def parseString(value):
    return doParse(Stream(input = value))

