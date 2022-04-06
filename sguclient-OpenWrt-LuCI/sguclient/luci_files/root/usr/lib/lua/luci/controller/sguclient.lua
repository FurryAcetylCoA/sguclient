module("luci.controller.sguclient", package.seeall)

function index()
        if not nixio.fs.access("/etc/config/sguclient")  then
            call("act_reset")
        end
        local page

        page =entry({"admin", "network", "sguclient"}, firstchild(), _("SGUClient LuCI"), 80)

        entry({"admin", "network", "sguclient","client"}, cbi("sguclient/sguclient"), _("SGUClient LuCI"), 1)


       
end

function act_reset()
        --Not impl yet
end
