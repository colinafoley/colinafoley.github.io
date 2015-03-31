---
layout: post
title: Confluence 5.7 Search Styles
date: 2015-03-27 14:48:00
categories: confluence
---

With the release of Confluence 5.7, Atlassian made drastic changes to the global search styles.

#Custom Color Scheme
To revert these style changes in your custom color scheme, use the following:

Navigate to **Cog** > **General Configuration** > **Look and Feel** > **Colour Scheme** > **Custom Colour Scheme** > **Edit**

Then edit **Search Field Background** and **Search Field** Text to be #ffffff and #000000, respectively.

#Default Theme
Reverting the styles for the default Documentation Theme is more challenging. There is no way to override the color scheme within the GUI.

As a workaround, create and host a CSS file containing the following:
{% highlight css %}
body.theme-documentation .aui-header .aui-quicksearch input[type='text'],
body.theme-documentation .aui-header .aui-quicksearch input[type='text'][type='text']:focus {
  color: #000000;
  background-color: #FFFFFF;
}
body.theme-documentation .aui-header .aui-quicksearch input[type='text'],
body.theme-documentation .aui-header .aui-quicksearch input[type='text'][type='text']:focus {
}
{% endhighlight %}

Then, link to this CSS file in the **At end of the HEAD** under **Cog** > **General Configuration** > **Look and Feel** > **Custom HTML**.

{% highlight html %}
<link rel="stylesheet" href="//url.to/hosted/file.css" type="text/css"/>
{% endhighlight %}
