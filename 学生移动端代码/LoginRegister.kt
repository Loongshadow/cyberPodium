package com.example.cyberpodium_android

//实现登录注册

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.google.gson.Gson
import okhttp3.*
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject
import java.io.IOException
import kotlin.concurrent.thread


class LoginRegister :AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_loginregister)

        val txtLog2Reg: TextView = findViewById(R.id.log2reg);
        val btnLogReg: Button = findViewById(R.id.buttonLogReg);
        val accountTV:TextView = findViewById(R.id.editTextAccount);
        val passwdTV:TextView = findViewById(R.id.editTextPassword);

        // 设置点击监听器
        btnLogReg.setOnClickListener {
            // 调用函数（方法）
            val str = btnLogReg.text;
            if(str == "登录")
                LogRegFun("login");
            else if(str == "注册")
                LogRegFun("register");
        }

        txtLog2Reg.setOnClickListener {
            val str = txtLog2Reg.text;
            if(str == "注册"){
                uiLog2Reg("登录");
                //passwdTV.setText("Password");
                LogRegFun("firstregister"); //get new unoccupy account;

            }else if(str == "登录"){
                uiLog2Reg("注册");
            }

        }
    }

    private fun uiLog2Reg(nowstr:String){
        val txtLog2Reg: TextView = findViewById(R.id.log2reg);
        val btnLogReg: Button = findViewById(R.id.buttonLogReg);
        val accountTV:TextView = findViewById(R.id.editTextAccount);
        val passwdTV:TextView = findViewById(R.id.editTextPassword);
        if(nowstr == "登录"){
            btnLogReg.setText("注册");
            txtLog2Reg.setText("登录");
        }else{
            btnLogReg.setText("登录");
            txtLog2Reg.setText("注册");
        }
    }

    //登录注册
    private fun LogRegFun(status: String) {
        val txtLog2Reg: TextView = findViewById(R.id.log2reg);
        val btnLogReg: Button = findViewById(R.id.buttonLogReg);
        val accountTV:TextView = findViewById(R.id.editTextAccount);
        val passwdTV:TextView = findViewById(R.id.editTextPassword);
        if(status == "login"){
            val account = accountTV.text.toString();
            val passwd = passwdTV.text.toString();
            if(account.length==0||account=="Account") return;

            //network module
            val jsonData = """  
            {  
                "userID":"$account",
                "password": "$passwd"  
            }  
            """
            val serverURL = "http://43.138.243.228/cyberpodium/And_interface/Login.php";
            thread {
                try {
                    val client = OkHttpClient()
                    //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                    val requestBody = jsonData.toRequestBody();
                    val request = Request.Builder()
                        .url(serverURL)
                        .post(requestBody)
                        .build()
                    val call=client.newCall(request)
                    val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                    val requestData = response.body?.string()
                    val resultJSON = JSONObject(requestData);
                    if (requestData != null) {
                        Log.e("postSync", requestData)
                        val mystatus = resultJSON["status"];
                        if(mystatus == "success"){
                            val intent = Intent(this, MainActivity::class.java)
                            intent.putExtra("userID", account);
                            startActivity(intent)
                            finish() // 结束当前登录/注册活动
                        }else{
                            //登录失败
                        }
                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }else if(status == "register"){
            val account = accountTV.text.toString();
            val passwd = passwdTV.text.toString();
            if(account.length==0||account=="Account") return;
            val jsonData = """  
            {  
                "userID":"$account",
                "password": "$passwd"  
            }  
            """
            val serverURL = "http://43.138.243.228/cyberpodium/And_interface/Register.php";
            thread {
                try {
                    val client = OkHttpClient()
                    //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                    val requestBody = jsonData.toRequestBody();
                    val request = Request.Builder()
                        .url(serverURL)
                        .post(requestBody)
                        .build()
                    val call=client.newCall(request)
                    val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                    //val myresponse = response;
                    val requestData = response.body?.string()
                    val resultJSON = JSONObject(requestData);
                    if (requestData != null) {
                        Log.e("postSync", requestData)
                        val mystatus = resultJSON["status"];
                        if(mystatus == "success"){
                            runOnUiThread {
                                uiLog2Reg("注册");
                            }
                        }else{
                            //注册失败
                        }
                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }

        }else if(status == "firstregister"){
            val jsonData = """  
            {  
                "userID": 100000000,
                "password": "nothing"  
            }  
            """
            val serverURL = "http://43.138.243.228/cyberpodium/And_interface/Register.php";
            thread {
                try {
                    val client = OkHttpClient()
                    //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                    val requestBody = jsonData.toRequestBody();
                    val request = Request.Builder()
                        .url(serverURL)
                        .post(requestBody)
                        .build()
                    val call=client.newCall(request)
                    val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                    //val myresponse = response;
                    val requestData = response.body?.string()
                    val resultJSON = JSONObject(requestData);
                    if (requestData != null) {
                        Log.e("postSync", requestData)
                        val mystatus = resultJSON["status"];
                        if(mystatus == "success"){
                            val newID = resultJSON["userID"];
                            accountTV.setText(newID.toString());
                        }else{
                            //
                        }
                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }
    }

}

