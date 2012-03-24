<?php
$cookie_file = dirname(__FILE__)."/cookie_".md5(basename(__FILE__)).".txt"; // ����Cookie�ļ�����·�����ļ���

function vlogin($url,$data){ // ģ���¼��ȡCookie����
    $curl = curl_init(); // ����һ��CURL�Ự
    curl_setopt($curl, CURLOPT_URL, $url); // Ҫ���ʵĵ�ַ	        
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, 0); // ����֤֤����Դ�ļ��
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, 1); // ��֤���м��SSL�����㷨�Ƿ����
    curl_setopt($curl, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']); // ģ���û�ʹ�õ������
	curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1); // ʹ���Զ���ת
	curl_setopt($curl, CURLOPT_AUTOREFERER, 1); // �Զ�����Referer
	curl_setopt($curl, CURLOPT_POST, 1); // ����һ�������Post����
	curl_setopt($curl, CURLOPT_POSTFIELDS, $data); // Post�ύ�����ݰ�
	curl_setopt($curl, CURLOPT_COOKIEJAR, $GLOBALS['cookie_file']); // ���Cookie��Ϣ���ļ�����
	curl_setopt($curl, CURLOPT_COOKIEFILE, $GLOBALS['cookie_file']); // ��ȡ�����������Cookie��Ϣ
	curl_setopt($curl, CURLOPT_TIMEOUT, 30); // ���ó�ʱ���Ʒ�ֹ��ѭ��
    curl_setopt($curl, CURLOPT_HEADER, 0); // ��ʾ���ص�Header��������
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1); // ��ȡ����Ϣ���ļ�������ʽ����
    $tmpInfo = curl_exec($curl); // ִ�в���
	if (curl_errno($curl)) {
       echo 'Errno'.curl_error($curl);
	}
    curl_close($curl); // �ر�CURL�Ự
    return $tmpInfo; // ��������
}

function vget($url){ // ģ���ȡ���ݺ���
    $curl = curl_init(); // ����һ��CURL�Ự
    curl_setopt($curl, CURLOPT_URL, $url); // Ҫ���ʵĵ�ַ	        
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, 0); // ����֤֤����Դ�ļ��
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, 1); // ��֤���м��SSL�����㷨�Ƿ����
    curl_setopt($curl, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']); // ģ���û�ʹ�õ������
	curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1); // ʹ���Զ���ת
	curl_setopt($curl, CURLOPT_AUTOREFERER, 1); // �Զ�����Referer
	curl_setopt($curl, CURLOPT_HTTPGET, 1); // ����һ�������Post����
	curl_setopt($curl, CURLOPT_COOKIEFILE, $GLOBALS['cookie_file']); // ��ȡ�����������Cookie��Ϣ
	curl_setopt($curl, CURLOPT_TIMEOUT, 30); // ���ó�ʱ���Ʒ�ֹ��ѭ��
    curl_setopt($curl, CURLOPT_HEADER, 0); // ��ʾ���ص�Header��������
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1); // ��ȡ����Ϣ���ļ�������ʽ����
    $tmpInfo = curl_exec($curl); // ִ�в���
	if (curl_errno($curl)) {
       echo 'Errno'.curl_error($curl);
	}
    curl_close($curl); // �ر�CURL�Ự
    return $tmpInfo; // ��������
}

function vpost($url,$data){ // ģ���ύ���ݺ���
    $curl = curl_init(); // ����һ��CURL�Ự
    curl_setopt($curl, CURLOPT_URL, $url); // Ҫ���ʵĵ�ַ	        
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, 0); // ����֤֤����Դ�ļ��
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, 1); // ��֤���м��SSL�����㷨�Ƿ����
    curl_setopt($curl, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']); // ģ���û�ʹ�õ������
	curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1); // ʹ���Զ���ת
	curl_setopt($curl, CURLOPT_AUTOREFERER, 1); // �Զ�����Referer
	curl_setopt($curl, CURLOPT_POST, 1); // ����һ�������Post����
	curl_setopt($curl, CURLOPT_POSTFIELDS, $data); // Post�ύ�����ݰ�
	curl_setopt($curl, CURLOPT_COOKIEFILE, $GLOBALS['cookie_file']); // ��ȡ�����������Cookie��Ϣ
	curl_setopt($curl, CURLOPT_TIMEOUT, 30); // ���ó�ʱ���Ʒ�ֹ��ѭ��
    curl_setopt($curl, CURLOPT_HEADER, 0); // ��ʾ���ص�Header��������
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1); // ��ȡ����Ϣ���ļ�������ʽ����
    $tmpInfo = curl_exec($curl); // ִ�в���
	if (curl_errno($curl)) {
       echo 'Errno'.curl_error($curl);
	}
    curl_close($curl); // �ؼ�CURL�Ự
    return $tmpInfo; // ��������
}

function delcookie($cookie_file){ // ɾ��Cookie����
 @unlink($cookie_file); // ִ��ɾ��
}

// ʹ��ʵ��
if(!file_exists($cookie_file)) { // ���Cookie�Ƿ����
$str = vget('http://www.kalvin.cn/?action=login'); // ��ȡ��¼���ֵ
preg_match("/name=\"formhash\" value=\"(.*?)\"/is",$str,$hash); // ��ȡ��¼���ֵ
vlogin('http://www.kalvin.cn/post.php','action=dologin&formhash='.$hash[1].'&username=aaa&password=bbb&clientcode=ccc'); // ��¼��ȡCookie
}
echo vget('http://www.kalvin.cn/');
?>