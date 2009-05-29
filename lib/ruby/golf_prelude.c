#include "ruby/ruby.h"
#include "vm_core.h"

static const char prelude_name0[] = "<internal:golf_prelude>";
static const char prelude_code0[] =
"class Object\n"
"  @@golf_hash = {}\n"
"  def method_missing m, *a, &b\n"
"    t = @@golf_hash[ [m,self.class] ] ||= matching_methods(m)[0]\n"
"    if t && b\n"
"      __send__(t, *a) {|*args|\n"
"        b.binding.eval(\"proc{|golf_matchdata| $~ = golf_matchdata }\").call($~) if $~\n"
"        b.call(*args)\n"
"      }\n"
"    else\n"
"      t ? __send__(t, *a, &b) : super\n"
"    end\n"
"  end\n"
"\n"
"  def matching_methods(s='', m=callable_methods)\n"
"    r=/^#{s.to_s.gsub(/./){\"(.*?)\"+Regexp.escape($&)}}/\n"
"    m.grep(r).sort_by do |i|\n"
"      i.to_s.match(r).captures.map(&:size) << i\n"
"    end\n"
"  end\n"
"\n"
"  def self.const_missing c\n"
"    t = @@golf_hash[ [c,self.class] ] ||= matching_methods(c,constants)[0]\n"
"    t and return const_get(t)\n"
"    raise NameError, \"uninitialized constant #{c}\", caller(1)\n"
"  end\n"
"\n"
"  def shortest_abbreviation(s='', m=callable_methods)\n"
"    s=s.to_s\n"
"    our_case = (?A..?Z)===s[0]\n"
"    if m.index(s.to_sym)\n"
"      1.upto(s.size){|z|s.scan(/./).combination(z).map{|trial|\n"
"        next unless ((?A..?Z)===trial[0]) == our_case\n"
"        trial*=''\n"
"        return trial if matching_methods(trial,m)[0].to_s==s\n"
"      }}\n"
"    else\n"
"      nil\n"
"    end\n"
"  end\n"
"\n"
"  def callable_methods\n"
"    self.class == Object ? methods + private_methods : methods\n"
"  end\n"
"\n"
"  private\n"
"\n"
"  def h(a='H', b='w', c='!')\n"
"    puts \"#{a}ello, #{b}orld#{c}\"\n"
"  end\n"
"\n"
"  alias say puts\n"
"\n"
"  def do_while\n"
"    0 while yield\n"
"  end\n"
"\n"
"  def do_until\n"
"    0 until yield\n"
"  end\n"
"end\n"
"\n"
"class Array\n"
"  alias old_to_s to_s\n"
"  alias to_s join\n"
"end\n"
"\n"
"class FalseClass\n"
"  alias old_to_s to_s\n"
"  def to_s\n"
"    \"\"\n"
"  end\n"
"end\n"
"\n"
"class Integer\n"
"  alias each times\n"
"  include Enumerable\n"
"end\n"
"\n"
"class String\n"
"  alias / split\n"
"\n"
"  def to_a\n"
"    split('')\n"
"  end\n"
"\n"
"  (Array.instance_methods-instance_methods-[:to_ary,:transpose,:flatten,:flatten!,:compact,:compact!,:assoc,:rassoc]).each{|meth|\n"
"    eval\"\n"
"    def #{meth}(*args, &block)\n"
"      a=to_a\n"
"      result = a.#{meth}(*args, &block)\n"
"      replace(a.join)\n"
"      if result.class == Array\n"
"        Integer===result[0] ? result.pack('c*') : result.join\n"
"      elsif result.class == Enumerator\n"
"        result.map(&:join).to_enum\n"
"      else\n"
"        result\n"
"      end\n"
"    end\"\n"
"  }\n"
"end\n"
"\n"
"class Enumerator\n"
"  alias old_to_s to_s\n"
"  (Array.instance_methods-instance_methods-[:replace]+[:to_s]).each{|meth|\n"
"    eval\"\n"
"    def #{meth}(*args, &block)\n"
"      to_a.#{meth}(*args, &block)\n"
"    end\"\n"
"  }\n"
"  alias old_inspect inspect\n"
"  alias inspect old_to_s\n"
"end\n"
;

void
Init_golf(void)
{
  rb_iseq_eval(rb_iseq_compile(
    rb_str_new(prelude_code0, sizeof(prelude_code0) - 1),
    rb_str_new(prelude_name0, sizeof(prelude_name0) - 1),
    INT2FIX(1)));

#if 0
    puts(prelude_code0);
#endif
}
