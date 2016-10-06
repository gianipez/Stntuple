#!/usr/bin/env python
#
#
# $Id: SConscript,v 1.1 2014/06/13 06:19:52 murat Exp $
# $Author: murat $
# $Date: 2014/06/13 06:19:52 $
#
import os, re, string, subprocess
Import('env')
#------------------------------------------------------------------------------
# last two components of the path. Ex: /not/this/but/THIS/AND_THIS
#                                      "AND_THIS" is usually "src"
#------------------------------------------------------------------------------
x = subprocess.call('scripts/build_config',shell=True)
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------

def stntuple_gen_rootcint(source, target, env, for_signature):
#    print "\n>>> stntuple_gen_rootcint called:"
#    print ">>> source = ",len(source),str(source[0]), str(source[1])
#    print ">>> target = ",len(target),str(target[0])+'\n'

    class_include = str(source[1]);
    linkdef       = str(source[0]);

#    print "[stntuple_gen_rootcint] class_include = %s"%class_include
#    print "[stntuple_gen_rootcint] linkdef       = %s"%linkdef
    
    includes =   "-Iinclude -I"+os.environ['ART_INC'     ];
    includes = includes + " -I"+os.environ['CETLIB_INC'  ];
    includes = includes + " -I"+os.environ['CANVAS_INC'  ];
    includes = includes + " -I"+os.environ['FHICLCPP_INC'];

    cmd = 'if [ ! -d '+tmpdir+' ] ; then mkdir -p '+tmpdir+'; fi ;';
    cmd = cmd+"rootcint -f "+str(target[0])+" -c -DMU2E "+includes+" "+class_include+" "+linkdef+"; ";

#    print ">>> cmd = %s"%cmd
    return cmd

stntuple_my_rootcint = Builder(generator     = stntuple_gen_rootcint,
                               single_source = 0,
                               suffix        = '.o',
                               src_suffix    = '.h')

env.Append(BUILDERS = {'StntupleRootCint' : stntuple_my_rootcint})




class stntuple_helper:
    """mu2e_helper: class to produce library names"""
#   This appears to behave like c++ static member and is initialized at class defintion time.
    sourceroot =  os.path.abspath('.')
#
#   Accesor
#
    def base(self):
        return self.sourceroot

    def generate_dictionaries(self,list_of_linkdef_files):
        list = []
        for f in list_of_linkdef_files:
            linkdef       = string.split(str(f),'/');
            clname        = string.replace(linkdef[len(linkdef)-1],"_linkdef.h","");
            include       = subdir+'/'+clname+'.hh';
            dict          = '#/tmp/src/'+d1+'/'+clname+'_dict.cxx';
            list.append(dict);
            env.StntupleRootCint(dict,[f,include]) 
        return list
        

    def make_list_of_obj_files (self, list_of_dict_files):
        list = []
        for dict in list_of_dict_files:
            o_cxx_file = string.replace(dict,".cxx",".o");
            list.append(o_cxx_file);
        return list

# Export the class so that it can be used in the SConscript files
# For reasons even Rob does't understand, this must come before the env.SConscript(ss) line.
Export('stntuple_helper')






# sb = stntuple_helper();


