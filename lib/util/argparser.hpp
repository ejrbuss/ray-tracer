#pragma once

using namespace std;

typedef function<i32(i32, const char**)> Validator;

struct Arg {
    const Validator validator;
    const string verbose;
    const string flag;
    const string help;
};

struct Opt {
    const string verbose;
    const string flag;
    const string help;
    bool & opt;
};

class ArgParser {

    private:
        string name;
        string msg;
        string vrs;
        vector<Arg> args;
        vector<Opt> opts;

        auto as_string(const Arg & arg) const -> string {
            stringstream stream;
            stream  << "  " << setw(16)
             << left << arg.verbose << setw(4)
             << left << arg.flag
             << left << arg.help << endl;
             return stream.str();
        }

        auto as_string(const Opt & opt) const -> string {
            stringstream stream;
            stream  << "  " << setw(16)
             << left << opt.verbose << setw(4)
             << left << opt.flag
             << left << opt.help << endl;
             return stream.str();
        }

    public:

        ArgParser() {}

        ArgParser(const string & name = "", const string & m = "", const string & v = "") :
            name(name), msg(m), vrs(v)
        {}

        auto arg(
            const Validator & vld,
            const string & v,
            const string & f,
            const string & h = "",
            const i32 n      = 1
        ) -> void {
            args.push_back(Arg { vld, v, f, h});
        }

        auto opt(
            bool & ref,
            const string & v,
            const string & f,
            const string & h = "",
            const i32 n      = 1
        ) -> void {
            opts.push_back(Opt { v, f, h, ref});
        }

        auto parse(const i32 argc, const char* argv[]) -> void {
            i32 argn = 1;
            while(argn < argc) {

                const string next = argv[argn];
                const i32 current = argn;

                if(equal(next, "--help") || equal(next, "-h")) {
                    success(message() + usage() + help());
                }
                for(Arg & arg: args) {
                    if(equal(next, arg.verbose) || equal(next, arg.flag)) {
                        argn++;
                        argn += arg.validator(argn, argv);
                    }
                }
                for(Opt & opt: opts) {
                    if(equal(next, opt.verbose) || equal(next, opt.flag)) {
                        opt.opt = true;
                        argn++;
                    }
                }
                if(argn == current) {
                    cout << "Unknown command line argument " << argv[argn] << endl;
                    usage();
                    fail();
                }
            }

        }

        auto help() const -> string {
            stringstream stream;
            stream << "\nOptions:" << endl;
            for(const Arg & arg: args) {
                stream << as_string(arg);
            }
            for(const Opt & opt: opts) {
                stream << as_string(opt);
            }
            return stream.str() + "\n";
        }

        auto usage() const -> string {
            return "\nUsage:\n  $ " + name + " [options]\n";
        }

        auto version() const -> string {
            return vrs;
        }

        auto message() const -> string {
            return msg;
        }
};