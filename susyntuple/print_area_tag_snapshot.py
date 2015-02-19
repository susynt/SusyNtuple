#!/bin/env python

# print a summary of the tags for all packages in the RootCore area

# davide.gerbaudo@gmail.com
# Feb 2015

from utils import first, getCommandOutput, rootcoredir
import os

def main():
    blacklist_dirs = ['bash', '.git', 'susynt_xaod_timing', '.RootCoreBin']
    base_dir = os.path.abspath(rootcoredir()+'/../')
    print "Info about ",base_dir
    pkg_dirs = [d for d in os.listdir(base_dir) if os.path.isdir(os.path.join(base_dir, d)) and d not in blacklist_dirs]
    pkg_dirs = [os.path.join(base_dir, d) for d in pkg_dirs]
    pkg_names = [os.path.basename(p.rstrip('/')) for p in pkg_dirs]
    max_pkg_name_len = max(len(p) for p in pkg_names)
    for pkg, pkg_name in zip(pkg_dirs, pkg_names):
        msg = svn_info(pkg) if is_svn_dir(pkg) else git_info(pkg) if is_git_dir(pkg) else "{0} ?".format(pkg)
        # print ('%'+str(max_pkg_name_len)+'s')%pkg_name.ljust(max_pkg_name_len)+' : '+msg
        print pkg_name.ljust(max_pkg_name_len)+' : '+msg

def is_svn_dir(path):
    svn_hidden = path+'/.svn'
    return os.path.isdir(path) and os.path.isdir(svn_hidden)

def is_git_dir(path):
    git_hidden = path+'/.git'
    return os.path.isdir(path) and os.path.isdir(git_hidden)

def svn_info(path):
    # get tag or last rev
    cmd = 'svn info '+path
    out = getCommandOutput(cmd)
    url_origin = first([l for l in out['stdout'].splitlines() if 'URL: ' in l])
    url_origin = url_origin.replace('/', ' ').split() if url_origin else []
    tag = url_origin[url_origin.index('tags')+1] if 'tags' in url_origin else None
    last_rev = first(first([l for l in out['stdout'].splitlines() if 'Last Changed Rev:' in l]).split()[::-1])
    # get list of modified files
    cmd = 'svn status '+path
    out = getCommandOutput(cmd)
    modified_files = [l for l in out['stdout'].splitlines() if l.startswith('M ')]
    return "{0} {1}".format(tag if tag else last_rev,
                            ('\n'.join(['']+modified_files) if modified_files else ''))

def git_info(path):
    cmd = 'git describe --tags --dirty'
    out = getCommandOutput(cmd)
    tag_info = out['stdout'].strip()
    modified_files = [l for l in getCommandOutput('git status', cwd=path)['stdout'].splitlines()
                      if 'modified:' in l] if 'dirty' in tag_info else []
    return "{0} {1}".format(tag_info,
                            ('\n'.join(['']+modified_files) if modified_files else ''))

#____________________________________________________________

if __name__=='__main__':
    main()
