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
env['CXXFLAGS'].append('-I'+os.environ['MU2E_BASE_RELEASE']+'/include');
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------
def stntuple_codegen_script(source, target, env, for_signature):
#    print "\n>>> mod_codegen called: pwd = "+os.environ['PWD'];
#    print ">>> source = ",len(source),str(source[0])
#    print ">>> target = ",len(target),str(target[0])+'\n'
#    print ">>> env[\'MU2E_BASE_RELEASE\']:"+os.environ['MU2E_BASE_RELEASE']
    
    tmpdir        = os.path.dirname(str(target[0]));
    
    cmd = 'if [ ! -d '+tmpdir+' ] ; then mkdir -p '+tmpdir+'; fi ; ';

    cmd = cmd+'source '+os.environ['MU2E_BASE_RELEASE']+'/Stntuple/scripts/create_print_header_routine.sh '+str(target[0]);

#    print ">>> cmd = %s"%cmd
    return cmd


stntuple_codegen = Builder(generator     = stntuple_codegen_script,
                           single_source = 0,
                           suffix        = '.cc',
                           src_suffix    = '.sh')

env.Append(BUILDERS = {'StntupleCodegen' : stntuple_codegen})
#------------------------------------------------------------------------------
def stntuple_gen_rootcint(source, target, env, for_signature):
#    print "\n>>> stntuple_gen_rootcint called:"
#    print ">>> source = ",len(source),str(source[0]), str(source[1])
#    print ">>> target = ",len(target),str(target[0])

    class_include = str(source[1]);
    linkdef       = str(source[0]);

#    print "[stntuple_gen_rootcint] class_include = %s"%class_include
#    print "[stntuple_gen_rootcint] linkdef       = %s"%linkdef
    
    includes =   "-Iinclude -I"+os.environ['ART_INC'     ];
    includes = includes + " -I"+os.environ['BTRK_INC'    ];
    includes = includes + " -I"+os.environ['CETLIB_INC'  ];
    includes = includes + " -I"+os.environ['CETLIB_EXCEPT_INC'];
    includes = includes + " -I"+os.environ['CANVAS_INC'  ];
    includes = includes + " -I"+os.environ['FHICLCPP_INC'];
    includes = includes + " -I"+os.environ['CLHEP_INC'   ];
    includes = includes + " -I"+os.environ['BOOST_INC'   ];

    dict     = str(target[0]);
    tmp_lib_dir = os.path.dirname(dict);
    
    pcm_file = dict.replace(".cxx","_rdict.pcm");

#    print "dict:"+dict + "   pcm_file:"+pcm_file;
    lib_dir = os.environ['MU2E_BASE_RELEASE']+"/lib";
    cmd = 'if [ ! -d '+tmp_lib_dir+' ] ; then mkdir -p '+tmp_lib_dir+'; fi ;';
    cmd = cmd+"rootcint -f "+dict+" -c  -D_CODEGEN_ -DMU2E "+includes+" "+class_include+" "+linkdef+"; ";
    cmd = cmd+'if [ ! -d '+lib_dir+' ] ; then mkdir '+lib_dir+' ; fi ; ';
    cmd = cmd+"mv "+pcm_file+" "+lib_dir+'/.'; 
#    print ">>> cmd = %s"%cmd
    return cmd

stntuple_my_rootcint = Builder(generator     = stntuple_gen_rootcint,
                               single_source = 0,
                               suffix        = '.o',
                               src_suffix    = '.h')

env.Append(BUILDERS = {'StntupleRootCint' : stntuple_my_rootcint})
#------------------------------------------------------------------------------

class stntuple_helper:
    """mu2e_helper: class to produce library names"""
#   This appears to behave like c++ static member and is initialized at class defintion time.
    sourceroot =  os.path.abspath('.')

    def __init__(self, debug=False):
        self._list_of_object_files = [];

        self.dd      = re.search('[^/]*/[^/]*$',env.Dir('.').abspath).group(0)
        self.dirname = os.path.dirname(self.dd);   # THIS
        self.subdir  = os.path.basename(self.dd);
        self.libname = self.dirname+'_'+self.subdir
        self.d1      = self.libname+'-shared';
        self.tmp_dir = "tmp/src/"+self.d1;
        self._debug  = debug
        if (debug) : print "-------------- building directory: "+self.dirname+'/'+self.subdir
#
#   Accesor
#
    def base(self):
        return self.sourceroot

    def tmp_dira(self):
        return self.tmp_dir

    def debug(self):
        return self._debug

    def do_codegen(self,cc,script):
        env.StntupleCodegen(cc,script);
        obj = string.replace(cc,".cc",".o");
        env.SharedObject(obj,cc)
        self._list_of_object_files.append(obj);

    def handle_dictionaries(self,skip_list = []):
#------------------------------------------------------------------------------
# generate dictionaries
#------------------------------------------------------------------------------
        list_of_linkdef_files = Glob(self.subdir+'/dict/*_linkdef.h', strings=True)
        if (self._debug):
            print ("[Stntuple."+self.subdir+"] handle_dictionaries: list_of_linkdef_files = \n",list_of_linkdef_files)
            
        list_of_dict_files    = []

        for f in list_of_linkdef_files:
            linkdef       = string.split(str(f),'/');
            linkdef_fn    = linkdef[len(linkdef)-1];

            if (self._debug): 
                print ("linkdef_fn = ",linkdef_fn)

            if (not linkdef_fn in skip_list):
                clname        = string.replace(linkdef_fn,"_linkdef.h","");
                include       = self.subdir+'/'+clname+'.hh';
            
                dict          = '#/tmp/src/'+self.d1+'/'+clname+'_dict.cxx';
                list_of_dict_files.append(dict);

                if (self._debug):
                    print ("linkdef = ",linkdef)
                    print ("include = ",include)

                env.StntupleRootCint(dict,[f,include])
#------------------------------------------------------------------------------
# compile dictionaries
#------------------------------------------------------------------------------
        list   = [];

        for dict in list_of_dict_files:
            obj_cxx_file = string.replace(dict,".cxx",".o");
            list.append(obj_cxx_file);
            self._list_of_object_files.append(obj_cxx_file);
        
            include  = string.replace(dict,".cxx",".h");
            env.SharedObject(obj_cxx_file,dict)


    def build_libs(self,list_of_cc_files, skip_list = [],libs = []):
        if (self._debug):
            print ("[Stntuple.build_libs]: list_of_cc_files:"+self.subdir,list_of_cc_files)

        for cc in list_of_cc_files:
            if (not cc in skip_list):
                if (self._debug):
                    print (".cc file: "+cc)
                    
                o = '#/tmp/src/'+self.d1+'/'+string.split(cc,'.')[0]+'.o'
                self._list_of_object_files.append(o);
                env.SharedObject(o,cc)

        #        libs     = [ 'Stntuple_base' ]

        lib_name = os.environ['MU2E_BASE_RELEASE']+'/lib/'+self.libname+'.so';

        if (self._debug):
            print ("Stntuple/obj list_of_object_files:",self._list_of_object_files)

        env.SharedLibrary(lib_name,self._list_of_object_files,LIBS = [libs])


    def build_modules(self,list_of_module_files, skip_list, libs = []):
        if (self._debug):
            print ("[Stntuple.build_libs]: list_of_module_files in Stntuple/"+self.subdir," : ",list_of_module_files)

        for module in list_of_module_files:
            if (not module in skip_list):
                if (self._debug):
                    print ("module file: "+module)
                    
                o = '#/tmp/src/'+self.d1+'/'+string.split(module,'.')[0]+'.o'
                env.SharedObject(o,module)

                mname = string.split(os.path.basename(module),'.')[0];
                lib   = '#/lib/libmu2e_'+self.dirname+'_'+mname+'.so';
                if (self._debug):
                    print ("o: "+o, "lib:"+lib)
                    
                env.SharedLibrary(lib,o,LIBS = ['libStntuple_mod.so',libs]);

# Export the class so that it can be used in the SConscript files
# For reasons even Rob does't understand, this must come before the env.SConscript(ss) line.
Export('stntuple_helper')

