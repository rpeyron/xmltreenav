<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:param name="filter" select="*[not(@diff:status)]|*[@diff:status='added']|*[@diff:status='removed']|*[count(descendant::*[not(@diff:status) or @diff:status='added' or @diff:status='removed'])&gt;0]" />

<xsl:include href="helper-default-filter.xsl" />

</xsl:stylesheet>