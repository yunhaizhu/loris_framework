import jinja2
import json
import os
import secrets
import sys

from jinja2 import nodes
from jinja2.ext import Extension

templateLoader = jinja2.FileSystemLoader(searchpath="./template")
templateEnv = jinja2.Environment(loader=templateLoader, trim_blocks=True, lstrip_blocks=True)


def json_data_init(data):
    f = open('target/IID.json', )
    json_data = json.load(f)
    f.close()

    if json_data.get(data['module']) is None:
        data['IID'] = '0x' + secrets.token_hex(4) \
                      + ', 0x' + secrets.token_hex(2) \
                      + ', 0x' + secrets.token_hex(2) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1) \
                      + ', 0x' + secrets.token_hex(1)

        json_data[data['module']] = data['IID']

        file1 = open('target/IID.json', "w+")
        file1.write(json.dumps(json_data, indent=4))
        file1.close()
    else:
        data['IID'] = json_data[data['module']]

    print(data['IID'])

    ORIG_TARGET = 'target/orig/' + data['version'] + '/' + data['module']

    template_files = [
        {
            'template_file': "pub_desc.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/public/desc.json'
        },
        {
            'template_file': "module_pub_inc_h.jinja2",
            'output_file': ORIG_TARGET + '/public/include/' + data['module'] + '.h'
        },
        {
            'template_file': "module_CMakeListsTxt.jinja2",
            'output_file': ORIG_TARGET + '/CMakeLists.txt'
        },
        {
            'template_file': "pri_inc_inst_h.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/private/include/' + data['instance'] + '.h'
        },
        {
            'template_file': "pri_src_CMakeListsTxt.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/CMakeLists.txt'
        },
        {
            'template_file': "pri_src_bundle_c.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/private/src/' + data['instance'] + '_bundle.c'
        },
        {
            'template_file': "pri_src_inst_c.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/private/src/' + data['instance'] + '.c'
        },
        {
            'template_file': "pri_src_stub_shell_c.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/private/src/stub/stub_shell_' + data['instance'] + '.c'
        },
        {
            'template_file': "pri_script_nl.jinja2",
            'output_file': ORIG_TARGET + '/' + data['instance'] + '/private/script/' + data['instance'] + '_test.nl'
        },
        # {
        #     'template_file': "pri_src_stub_rpc_call_c.jinja2",
        #     'output_file': ORIG_TARGET+'/private/src/stub/stub_rpc_'+data['instance']+'_call.c'
        # },
        # {
        #     'template_file': "pri_src_stub_rpc_c.jinja2",
        #     'output_file': ORIG_TARGET+'/private/src/stub/stub_rpc_'+data['instance']+'.c'
        # },
        # {
        #     'template_file': "pub_include_test_case_json.jinja2",
        #     'output_file': ORIG_TARGET+'/public/include/'+data['instance']+'.json'
        # }
    ]
    return template_files, ORIG_TARGET, data['instance']


def render_template_file(template_files, data):
    for item in template_files:
        template = templateEnv.get_template(item['template_file'])
        outputText = template.render(data)

        print(item['template_file'])
        file1 = open(item['output_file'], "w+")
        file1.write(outputText)
        file1.close()


def indent_template_file(template_files, data):
    for item in template_files:
        if item['template_file'] != 'pub_desc.jinja2' and \
                item['template_file'] != "module_CMakeListsTxt.jinja2" and \
                item['template_file'] != "pri_src_CMakeListsTxt.jinja2" and \
                item['template_file'] != "pri_script_nl.jinja2" and \
                item['template_file'] != "pub_include_test_case_json.jinja2":
            # cmd = 'astyle --style=linux -n '+item['output_file']
            cmd = 'indent -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -l120 -d0 -di1 -nfc1 -i8 -ip0 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1 ' + \
                  item['output_file']

            print(item['template_file'], cmd)
            os.system(cmd)

        if item['template_file'] == "pub_include_test_case_json.jinja2":
            f = open(item['output_file'], )
            json_data = json.load(f)
            f.close()
            print(json.dumps(json_data, indent=4))
            file1 = open(item['output_file'], "w+")
            file1.write(json.dumps(json_data, indent=4))
            file1.close()


def mkdir_p(filename):
    try:
        folder = os.path.dirname(filename)
        if not os.path.exists(folder):
            os.makedirs(folder)
        return True
    except:
        return False


def main():
    args = sys.argv[1:]
    print(args)

    mkdir_p('target')
    cmd = './json_output/json_output ' + args[0]
    print(cmd)
    os.system(cmd)

    f = open('target/output.json', )
    data = json.load(f)
    f.close()

    template_files, ORIG_TARGET, INSTANCE = json_data_init(data)

    mkdir_p(ORIG_TARGET)
    mkdir_p(ORIG_TARGET + '/' + INSTANCE + '/private/src')
    mkdir_p(ORIG_TARGET + '/' + INSTANCE + '/private/src/stub/')
    mkdir_p(ORIG_TARGET + '/' + INSTANCE + '/private/include/')
    mkdir_p(ORIG_TARGET + '/' + INSTANCE + '/private/script/')
    mkdir_p(ORIG_TARGET + '/' + INSTANCE + '/public/')
    mkdir_p(ORIG_TARGET + '/public/include/')
    mkdir_p(ORIG_TARGET + '/public/src/')

    render_template_file(template_files, data)
    cmd = 'find ' + ORIG_TARGET + ' -name "*.[c|h]"|xargs python3 ../add_doxygen_comment.py'
    os.system(cmd)

    indent_template_file(template_files, data)


if __name__ == "__main__":
    # execute only if run as a script
    main()
